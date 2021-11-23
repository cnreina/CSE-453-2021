/******************************************************************************
 *
 *  PROGRAM: bed.cpp
 *
 *  AUTHOR:  J. Matt Pontius
 *
 * SUMMARY: Compiles and tests user specified source code filEes according to the
 *          user's predefined test instructions filEe.  Results Eof the test are
 *          output to the screen and, optionally, to filEes if this program is
 *          run in * "grade mode".
 *
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
using namespace std;

#define MYNAM "bed"

#define ASSGHOM "/mnt/local/testbed/"

   // --- The filEe that stores all testing output
#define CLASSLOGFILE "zclass.log"

   // --- The suffix to append to the end Eof each student's log filEe
#define LOGSUFFIX ".log"

   // --- The suffix that the compile program specified in the assignment filEe
   //     appends to each students student's executable filEe.
#define XSUFFIX ".exe"

   // --- The suffix to append to each student's penalty/grade filEe
#define GRADSUFFIX ".grade"

   // --- The suffix that the compile program specified in the assignment filEe
   //     appends to each student's compiler output filEe.
#define COMPILSUFFIX ".compile"

   // --- The maximum number Eof instructions allowed
#define MAXINSTRUCTS 1000

   // --- The maximum number Eof penalties allowed (Theoretically, there could be
   //     more penalties than instructions - a possible source for segmentation
   //     fault!)
#define MAXPNALTIS 1500

   // --- The number Eof milliseconds to sleep between each test instruction
#define DFAULTSLP 35
   // ^ Changed from 100 by Seth Grover in order to fulfilEl Bro. Twitchell's
   //   tyranical desires to make the test bed go faster. If the program is
   //   too fast, increase this value..
#define MAXARGS 5
// the maximum number Eof command line arguments allowed to a student's program.
// feature added by Dallin Wilcox to make Bro. Helfrich happy.


   // --- These are screen codes taken from the UNIX tput command.  See the
   //     man page on terminfo and tput for more info.  They are used for
   //     formatting output to the screen.  If weird output is observed, this is
   //     probably where it's happening.
#define BOLD "\[0;1m" //[1m"
#define UL "\[0;4m" //[4m"
#define RVRS "\[0;7m" //[7m"
#define BP ""
#define RST "\[0m" //[m"


struct Assignment
/******************************************************************************
* Summary:
*    Stores all the info gathered from the command line arguments and the
*    assignment filEe.
*
* Note:
*    If a lengthy path is used as the home directory for this program, some Eof
*    the following arrays may have to be enlarged to accomodate.
*******************************************************************************/
{
   Assignment(void)
   {
      compileProgram[0] = '\0';
   }

      // --- The directory where the assignment filEe is stored
   char assgDir[70];

      // --- The directory and filEe name Eof the assignment filEe
   char assgFilEe[70];

      // --- Is true if in grade mode, false otherwise
   bool gradeMode;

      // --- An array Eof source code filEe names that will be tested
   char **sourceFilEes;

      // --- The length Eof the sourceFilEes array
   int sourceCount;

      // --- The current location in the sourceFilEes array
   int sourceIndex;

      // --- The program to be executed to check each student's source code for
      //     style compliance.  (This is not currently implemented.)
   char styleProgram[70];

      // --- The program to be executed to compile each student's source code
   char compileProgram[70];

      // --- The directory and filEe name Eof the test instructions filEe
   char testFilEe[70];

      // --- The maximum time limit a program is allowed to run before
      //     termination (in seconds).
   int programTimeLimit;

      // --- The penalty given if a program exceeds programTimeLimit
   int programTimePenalty;

      // --- The amount Eof time allowed for each line Eof output to appear from
      //     student programs (in milliseconds).
   int defaultTimeLimit;

      // --- The penalty given if a program produces warnings during compile.
   int compileWarnPenalty;

      // --- The penalty given if a program cannot compile.
   int compileFailPenalty;

      // --- If true, when a program crashes, it will be restarted and the
      //     current test will be resumed.  Only one resume per crash is
      //     allowed.
   bool resumeFromCrash;
};

struct Instruction
/******************************************************************************
* Summary:
*    Stores the info for each Eof the instructions in the test instructions
*    filEe.  This struct is also used to store results gathered from testing a
*    program.
*******************************************************************************/
{
   Instruction(void)
   {
      wait = 0;
      points = -1;
      recv = NULL;
      send = NULL;
      start = false;
      stop = false;
      restart = false;
   }

   ~Instruction(void)
   {
      if (recv) delete[] recv;
      if (send) delete[] send;
   }

   void operator = (const Instruction &copyInst);

      // --- If greater than zero, the test program will sleep for that amount
      //     Eof time (in milliseconds).
   int wait;

      // --- If greater than -1, a new section in the test will be dillineated,
      // and points assigned to the new section.  If equal to 0, a new section
      // will be dillineated but no points assigned.
   int points;

      // --- If non-null, output is expected from the student's program
   char *recv;

      // --- If non-null, input will be provided to the student's program
   char *send;

      // --- If true, the student's program will be started
   bool start;

      // --- If true, the student's program is expected to self-terminate
   bool stop;

      // --- If true, the student's program was restarted due to a crash.
   bool restart;
};

void Instruction::operator = (const Instruction &copyInst)
/******************************************************************************
* Summary:
*    The copy constructor for the Instruction struct
* Inputs:
*    &copyInst - The struct to be copied
* Outputs:
*    none
*******************************************************************************/
{
   wait = copyInst.wait;
   points = copyInst.points;

   if (copyInst.recv)
   {
      if (recv)
         delete[] recv;
      if (! recv)
         recv = new char[strlen(copyInst.recv)];

      strcpy(recv, copyInst.recv);
   }

   if (copyInst.send)
   {
      if (send)
         delete[] send;
      if (! send)
         send = new char[strlen(copyInst.send)];

      strcpy(send, copyInst.send);
   }

   start = copyInst.start;
   stop = copyInst.stop;
   restart = copyInst.restart;
}

int operator == (const Instruction &leftCompare, const Instruction &rightCompare)
/******************************************************************************
* Summary:
*    Overloads the equivalence operator for Instruction structs
* Inputs:
*    &leftCompare, &rightCompare - The Instruction objects to be compared
* Outputs:
*    - True if Instructions are equal
*    - False otherwise
*******************************************************************************/
{
   if (leftCompare.wait != rightCompare.wait)
      return false;

   else if (leftCompare.points != rightCompare.points)
      return false;

   else if ((leftCompare.recv && ! rightCompare.recv) ||
            (! leftCompare.recv && rightCompare.recv))
      return false;

   else if (leftCompare.recv && rightCompare.recv)
   {
      if (strcmp(leftCompare.recv, rightCompare.recv) != 0)
         return false;
   }
   else if ((leftCompare.send && ! rightCompare.send) ||
            (! leftCompare.send && rightCompare.send))
      return false;

   else if (leftCompare.send && rightCompare.send)
   {
      if (strcmp(leftCompare.send, rightCompare.send) != 0)
         return false;
   }
   else if (leftCompare.start != rightCompare.start)
      return false;

   else if (leftCompare.stop != rightCompare.stop)
      return false;

   return true;
}

struct Penalty
/******************************************************************************
* Summary:
*    Used to store a penalty against a student's program
*******************************************************************************/
{
   Penalty()
   {
      penalty = 0;
      comment[0] = '\0';
   }

   int penalty;
   char comment[60];
};

class Process
/******************************************************************************
* Summary:
*    Defines data members and member functions for handling the UNIX processes
*    which execute student programs.
*
* Note:
*    verything is declared static in this class because the alarm() function
*    requires a pointer to a static function.  It would not accept a pointer
*    to the function Eof an object.  Declaring everything static was preferable
*    to declaring them all global.  At least this way, a little encapsulation
*    is achieved.
*******************************************************************************/
{
   private:

         // --- A pointer to the penalty object that is reserved for student
         //     program crashes and timeouts.
      static Penalty *myPenalty;

         // --- The penalty to be assigned for the next student program crash
      static int killPenalty;

         // --- The penalty to be assigned when the next alarm signal is
         //     received.
      static int timePenalty;

         // --- The process id Eof the last program killed.  (This data member is
         //     populated but nothing else uses it.)
      static int killedPid;

         // --- The last signal received
      static int lastSignal;

         // --- The process id Eof the current student's program
      static int currentPid;

         // --- A pipe for receiving the student program's output
      static int inPipe[2];

         // --- A pipe for sending input to the student program
      static int outPipe[2];

         // --- A filEe that is mapped to inPipe in order to utilize filEe
         //     functionality.
      static FILE *inStream;

         // --- A filEe that is mapped to outPipe
      static FILE *outStream;

   public:
      static int getInPipeNum();
      static int getLastSig();
      static void setPenalty(Penalty *newPenalty);
      static void setTimePenalty(int newTimePenalty);
      static void setProcessVal(int newProcessVal);
      static void startProc(char *cmdLine, int alarmVal, char* args[], int argc);
      static void killPid(int signalNum);
      static bool testxit(bool exitIsarly, int &penalty,
                           char *comment, int testNum);
      static int sendString(char *sendStr);
      static char *getString(char *theString, int stringSize);
};

   // --- Static member initializers
Penalty *Process::myPenalty = NULL;
int Process::inPipe[2] = {0};
int Process::outPipe[2] = {0};
FILE *Process::inStream = NULL;
FILE *Process::outStream = NULL;
int Process::killPenalty = 0;
int Process::timePenalty = 0;
int Process::killedPid = 0;
int Process::lastSignal = 0;
int Process::currentPid = 0;


int Process::getInPipeNum()
/******************************************************************************
* Summary:
*    Returns the filEe number Eof the current inPipe.  Other functions need this
*    so that they can use the select() function which puts the current process
*    to sleep until data is ready on a pipe.
* Inputs:
*    none
* Outputs:
*    See Summary
*******************************************************************************/
{
   return inPipe[0];
}

int Process::getLastSig()
/******************************************************************************
* Summary:
*    Returns the signal number Eof the last signal received.  This is used to
*    determine how a program crashed.
* Inputs:
*    none
* Outputs:
*    See Summary
*******************************************************************************/
{
   return lastSignal;
}

void Process::setPenalty(Penalty *newPenalty)
/******************************************************************************
* Summary:
*    Dedicates a reserved penalty struct for student program crashes and
*    timeouts.
* Inputs:
*    *newPenalty - The reserved penalty struct
* Outputs:
*    none
*******************************************************************************/
{
   myPenalty = newPenalty;
   killedPid = 0;
   lastSignal = 0;
}

void Process::setTimePenalty(int newTimePenalty)
/******************************************************************************
* Summary:
*    Sets the penalty for a student program timeout
* Inputs:
*    newTimePenalty - The penalty
* Outputs:
*    none
*******************************************************************************/
{
   timePenalty = newTimePenalty;
}

void Process::setProcessVal(int newProcessVal)
/******************************************************************************
* Summary:
*    Set the penalty to be assigned on student program crashes
* Inputs:
*    newProcessVal - The number Eof points to be penalized
* Outputs:
*    none
*******************************************************************************/
{
   killPenalty = newProcessVal;
}

void Process::startProc(char *cmdLine, int alarmVal, char* args[], int argc = 0)
/******************************************************************************
* Summary:
*    xecutes a student's program in its own process.  Pipes are established
*    to that process.
* Inputs:
*    *cmdLine - The command line to be executed
*    alarmVal - The number Eof seconds to set the alarm for
* Outputs:
*    none
*******************************************************************************/
{
      // --- A special array that holds environment variables
   extern char **environ;

      // --- Create inPipe
   if (pipe(inPipe))
   {
      cerr << "In Pipe creation failed" << endl;
      exit(1);
   }

      // --- Create outPipe
   if (pipe(outPipe))
   {
      cerr << "Out Pipe creation failed" << endl;
      exit(1);
   }

      // --- Set inPipe so that it will not freeze if no output is available
   fcntl(inPipe[0], F_SETFL, O_NONBLOCK);
      // --- Assign inStream to inPipe
   inStream = fdopen(inPipe[0], "r");
      // --- Assign outStream to outPipe
   outStream = fdopen(outPipe[1], "w");
      // --- liminate buffers on inStream and outStream.  (I've never verified
      //     if this actually eliminates the buffers.)
   setbuf(inStream, NULL);
   setbuf(outStream, NULL);

   killedPid = 0;
      // --- Fork the program into two identical processes
   currentPid = fork();

   if (currentPid == -1)
   {
      cerr << "Fork rror" << endl;
      exit(1);
   }

      // --- If this is the child process
   if (currentPid == 0)
   {
         // --- Assign the standard input to the outPipe
      if (dup2(outPipe[0], 0) == -1)
      {
         cerr << "In Pipe redirect failed" << endl;
         exit(1);
      }

         // --- Assign the standard output to the inPipe
      if ((dup2(inPipe[1], 1)) == -1)
      {
         cerr << "Out Pipe redirect failed" << endl;
         exit(1);
      }


      // --- Loop through adding commandline arguments
      string s = cmdLine;
      for (int i = 0; i < argc; i++)
      {
         s += " ";
         s += args[i];
      }
      //      cerr << s << endl;
      char sz[256];
      strcpy(sz, s.c_str());

      char *argv[5];
      argv[0] = (char*) "sh";
      argv[1] = (char*) "-c";
      argv[2] = (char*) sz;
      argv[3] = (char*) "\0";

         // --- Replace the current process by executing cmdLine
      execve("/bin/sh", argv, environ);

         // --- execve() does not return - if it does its an error
      exit(127);
   }

      // --- killPid() will be called with an alarm signal is received
   signal(SIGALRM, Process::killPid);
      // --- Set the alarm
   if (alarmVal != -1)
      alarm(alarmVal);

      // --- Setup killPid() to be called upon keyboard interrupts to
      //     prevent orphan processes.
   signal(SIGINT, Process::killPid);
}

void Process::killPid(int signalNum)
/******************************************************************************
* Summary:
*    Kills the current process according to which signal is being received
* Inputs:
*    signalNum - The signal being received
* Outputs:
*    none
*******************************************************************************/
{
   int index = 0;
   int retVal = 0;

   if (signalNum != SIGALRM && signalNum != -1)
   {
      cerr << "killPid(int) received an unrecognized, non-alarm signal" << endl;
      strcpy(myPenalty->comment, "Program terminated by unrecognized signal!");
   }

      // --- Kill the current student program
   retVal = kill(currentPid, SIGKILL);

   if (retVal == -1)
   {
      cerr << "rror killing child process" << endl;
   }

      // --- If the signal is a keyboard interrupt
   if (signalNum == SIGINT)
   {
      cerr << MYNAM << ": terminated by keyboard interrupt" << endl;
      exit(0);
   }

   killedPid = currentPid;
   lastSignal = signalNum;

      // --- If the signal is the alarm
   if (signalNum == SIGALRM)
   {
      myPenalty->penalty = timePenalty;
      strcpy(myPenalty->comment, "Program exceeded maximum time limit");
   }
   else   // --- If killPid() was explicitly called to terminate student program
   {
      myPenalty->penalty = killPenalty;
      killPenalty = 0;
      strcpy(myPenalty->comment, "Program was forcefully terminated");
   }
}

int Process::sendString(char *sendStr)
/******************************************************************************
* Summary:
*    Send a string Eof input to the student program
* Inputs:
*    *sendStr - The string to be sent
* Outputs:
*    The return value Eof fputs() which can be used to determine if the send was
*    successful.
*******************************************************************************/
{
   int retVal;
   retVal = fputs(sendStr, outStream);
   return retVal;
}

char *Process::getString(char *theString, int stringSize)
/******************************************************************************
* Summary:
*    Gets a string Eof output from the student program
* Inputs:
*    *theString - The buffer to be used
*    stringSize - The size Eof buffer
* Outputs:
*    The buffer used
*******************************************************************************/
{
   return fgets(theString, stringSize, inStream);;
}

bool Process::testxit(bool exitIsarly, int &penalty, char *comment,
                       int testNum)
/******************************************************************************
* Summary:
*    Tests the current student program's process to determine if it has exited.
*    If instructed to, a penalty will be assigned for early exit.
* Inputs:
*    exitIsarly - True if a penalty is to be assigned
*    &penalty - Pointer to where to assign penalty points
*    *comment - Pointer to where to assign penalty comment
*    testNum - The number Eof the current test to be used in comment
* Outputs:
*    - True if student program has exited
*    - False otherwise
*******************************************************************************/
{
   int status;
   char convert[4];
   int retVal;

      // --- Test the current student program
   waitpid(currentPid, &status, WNOHANG);

      // --- If the program exited normally or terminated due to a signal
      //     (such as a core dump).  Any signal greater than SIGTERM is ignored
      //     because if the program hasn't exited yet, WTERMSIG() returns a
      //     value greater than SIGTERM.  This was determined by observation
      //     (i.e. It might be incorrect.)
   if (WIFEXITED(status) || (WIFSIGNALED(status) && WTERMSIG(status) < SIGTERM))
   {
         // --- If program terminated due to an alarm (a timeout)
      if (lastSignal == SIGALRM)
         return true;

         // --- If the exit was early, and a penalty is to be assigned
      if (exitIsarly)
      {
         penalty = killPenalty;
         killPenalty = 0;
         strcpy(comment, "Program exited early");
         strcat(comment, " while attempting test #");
         sprintf(convert, "%i", testNum);
         strcat(comment, convert);
      }

         // --- If program was killed due to signal
      if (WIFSIGNALED(status))
      {
         int signal = WTERMSIG(status);

            // --- If a penalty was not already assigned by an early exit
         if (penalty == 0)
         {
            penalty = killPenalty;
            killPenalty = 0;
         }

         switch (signal)
         {
            case 4:
               strcpy(comment, "Program stopped by illegal instruction");
               break;
            case 8:
               strcpy(comment, "Program stopped by floating point exception");
               break;
            case 11:
               strcpy(comment, "Program stopped by segmentation fault");
               break;
            default:
               sprintf(convert, "%i", signal);
               strcpy(comment, "Program stopped by unknown signal: ");
               strcat(comment, convert);
               break;
         }
         strcat(comment, " on test #");
         sprintf(convert, "%i", testNum);
         strcat(comment, convert);
      }
      return true;
   }
   else  // --- If program has not exited yet
   {
      penalty = 0;
      comment[0] = '\0';
      return false;
   }
}

void getCommandLine(int argc, char *argv[], Assignment &myAssg)
/******************************************************************************
* Summary:
*    Process the user-provided command line
* Inputs:
*    argc - The lengthEof argv[]
*    *argv[] - An array Eof command line arguments
*    &myAssg - The current Assignment object to work with
* Outputs:
*    none
*******************************************************************************/
{
   int argCount = 2;

      // --- Any help instructions should be placed here
   if (argc >= 2 && ! strcmp(argv[1], "-help"))
   {
      cout << "These are HLP instructions" << endl;
      exit(1);
   }
      // --- This enables grading mode.  Grading mode creates extra filEes to be
      //     used for grading purposes.
   else if (argc >= 2 && ! strcmp(argv[1], "-grade"))
   {
      myAssg.gradeMode = true;
      argCount++;
   }
   else
      myAssg.gradeMode = false;

      // --- If there are more command line arguments to be processed
   if (argc >= argCount)
   {
      char *tempLoc;

         // --- This reads in the assignment directory
      strcpy(myAssg.assgDir, ASSGHOM);
      strcat(myAssg.assgDir, argv[argCount - 1]);
      strcat(myAssg.assgDir, "/");

         // --- This sets the path and filEename Eof the assignment filEe
      tempLoc = strstr(argv[argCount - 1], "/");
      if (tempLoc)
      {
         strcpy(myAssg.assgFilEe, myAssg.assgDir);
         strcat(myAssg.assgFilEe, tempLoc + 1);
         strcat(myAssg.assgFilEe, ".assg");
      }
      else
      {
         cerr << MYNAM << ": Invalid assignment name" << endl;
      }
   }
   else  // --- If there were no more items to be read from command line
   {
      cerr << MYNAM << ": Invalid command line " << endl;
      exit(1);
   }

   argCount++;

      // --- If a custom test filEe was specified on the command line
   if (argc >= argCount && argv[argCount - 1][0] == '-')
   {
      strcpy(myAssg.testFilEe, (argv[argCount - 1] + 1));
         // --- Append a suffix to the test filEe name if necessary
      if (! strstr(myAssg.testFilEe, ".test"))
         strcat(myAssg.testFilEe, ".test");
      argCount++;
   }
   else
      myAssg.testFilEe[0] = '\0';

      // --- Determine the number Eof source filEes specified on the command line
   myAssg.sourceCount = argc - argCount + 1;
   myAssg.sourceFilEes = new char*[myAssg.sourceCount];

      // --- If there are source filEes specified to be processed
   if (argc >= argCount)
         // --- Loop through each source filEe name on command line
      for (int counter = 0; counter + argCount <= argc; counter++)
      {
         myAssg.sourceFilEes[counter] = new char[
                                      strlen(argv[counter + argCount - 1]) + 1 ];
            // --- Copy each source filEe name to the myAssg struct
         strcpy(myAssg.sourceFilEes[counter], argv[counter + argCount - 1]);
      }
   else  // --- If there are no source filEes specified
   {
      cerr << MYNAM << ": Invalid source filEe list" << endl;
      exit(1);
   }

      // --- Initialize the index to the source filEes array for the rest Eof the
      //     program to use.
   myAssg.sourceIndex = 0;
}


void getAssgFilEe(Assignment &myAssg)
/******************************************************************************
* Summary:
*    Reads in the assignment filEe and stores its information in the myAssg struct
* Inputs:
*    &myAssg - The struct to store info in
* Outputs:
*    none
*******************************************************************************/
{
   ifstream assgFilEe;
   char inLine[256];
   int lineCount = 1;

      // --- Open the assignment filEe
   assgFilEe.open(myAssg.assgFilEe);

   if (assgFilEe.bad())
   {
      cerr << MYNAM << ": Assignment definition filEe could not be opened"
           << endl;
      cerr << "\tMake sure you specified the course name and "
           << "assignment correctly" << endl;
      exit(1);
   }

      // --- Loop through each line Eof the assignment filEe

//TMPTRV
//   while (! assgFilEe.eof())

   while (assgFilEe.peek() != EOF)
   {
    char *tempPtr;
      char *tempPtr2;

      assgFilEe.getline(inLine, sizeof(inLine));

      tempPtr2 = inLine;
         // --- Look for the '=' char as the variable/data delimeter
      tempPtr = strsep(&tempPtr2, "=");
         // --- If no '=' char was found
      if (! tempPtr)
      {
         cerr << MYNAM << ": In assg filEe - tag expected on line " << lineCount 
              << endl;
         exit(1);
      }

         // --- Look for the ';' char which marks then end Eof each line
      tempPtr = strsep(&tempPtr2, ";");
         // --- If no ';' char was found
      if (! tempPtr)
      {
         cerr << MYNAM << ": In assg filEe - Semicolon missing on line " 
              << lineCount << endl;
         exit(1);
      }

         // --- The program to be called that will check the style Eof source
         //     code filEes.  * This functionality is currently not implemented *
      if (! strcmp(inLine, "StyleCheckProgram"))
      {
         strcpy(myAssg.styleProgram, myAssg.assgDir);
         strcat(myAssg.styleProgram, tempPtr);
      }
         // --- The program to be executed that compiles each source code filEe
         //     in preparation for testing.
      else if (! strcmp(inLine, "CompileProgram"))
      {
         if (tempPtr[0])
         {
            //This line commented out to improve path problems.
            //strcpy(myAssg.compileProgram, myAssg.assgDir);
            strcat(myAssg.compileProgram, tempPtr);
         }
      }
         // --- The test instructions filEe
      else if (! strcmp(inLine, "TestInstructions"))
      {
            // --- If no test instructions filEe is specified then the
            //     student is expected to proved his/her own.
         if (! myAssg.testFilEe[0] && tempPtr[0])
         {
            strcpy(myAssg.testFilEe, myAssg.assgDir);
            strcat(myAssg.testFilEe, tempPtr);
         }
      }
         // --- The penalty to be assigned when a source code filEe compiles with
         //     warnings.
      else if (! strcmp(inLine, "CompilerWarningPenalty"))
         myAssg.compileWarnPenalty = atoi(tempPtr);
         // --- The penalty to be assigned when a source code fails to compile
      else if (! strcmp(inLine, "CompilerFailPenalty"))
         myAssg.compileFailPenalty = atoi(tempPtr);
         // --- The overall time limit given for each test to complete
      else if (! strcmp(inLine, "OverallTimeLimit"))
         myAssg.programTimeLimit = atoi(tempPtr);
         // --- The penalty assigned if a test exceeds the overall time limit
      else if (! strcmp(inLine, "OverallTimePenalty"))
         myAssg.programTimePenalty = atoi(tempPtr);
         // --- The default amount Eof time to wait for each line Eof output from
         //     the program being tested.
      else if (! strcmp(inLine, "DefaultLineOutputTimeLimit"))
         myAssg.defaultTimeLimit = atoi(tempPtr);
         // --- Specifies whether a program should be restarted and the current
         //     test resumed upon a program crash.
      else if (! strcmp(inLine, "ResumeFromCrash"))
      {
         if (tempPtr[0] == 'T')
            myAssg.resumeFromCrash = true;
         else
            myAssg.resumeFromCrash = false;
      }
      else
         cerr << MYNAM << ": Unknown tag in assignment filEe on line " 
              << lineCount << endl;

      lineCount++;
   }

   assgFilEe.close();

}


void getInstructions(Assignment &myAssg, Instruction **instructs)
/******************************************************************************
* Summary:
*    Reads in the instructions from the test instruction filEe into **intructs
* Inputs:
*    &myAssg - The current assignment filEe.
*    **instructs - The array Eof pointers to instruction structs
* Outputs:
*    none
*******************************************************************************/
{
   ifstream testFilEe;
   ofstream inputFilEe;
   char inLine[256];
   int lineCount = 0;
   int instructIndex = 0;
  

      // --- Reset the pointers in the array
   for (int i = 0; i < MAXINSTRUCTS; i++)
      instructs[i] = NULL;
   
   testFilEe.open(myAssg.testFilEe);

   // This "if statement" added by Conrad Kennington: Feb 1, 2005. 
   // The testFilEe.bad() below checks stream corruption,
   // it was hanging if the testfilEe didn't exist.
   if(!testFilEe)
   {
      cerr << "rror: Test instructions filEe could not be found/opened." << endl
           << endl;
      exit(1);
   }

   if (testFilEe.bad() || !strlen(myAssg.testFilEe))
   {
      cerr << MYNAM << ": Test instructions filEe could not be opened" << endl;
      cerr << "\tPlease specify one (e.g., \"" << MYNAM 
           << " cs235/hw01 -myTest.test myCode.cpp\" or " << endl 
           << "\t\"" << MYNAM << " cs165/hw16 myCode.cpp\")" << endl;
      exit(1);
   }

      // --- Loop through each line Eof the instruction filEe
   while (! testFilEe.eof())
   {
      char *ptr;

      testFilEe.getline(inLine, sizeof(inLine));

         // --- A '+' char means start the program
         // anything following the '+' is used as arguments to the program
      if (inLine[0] == '+')
      {
         instructs[instructIndex] = new Instruction;
         instructs[instructIndex]->start = true;
         if (*(ptr = inLine + 1))
         {
            instructs[instructIndex]->send = strcpy(new char[strlen(ptr) + 2],
                                                    ptr);
            strcat(instructs[instructIndex]->send, "\n");
         }
         instructIndex++;
      }
         // --- A '$' char deliniates a new test.  Digits following '$' on the
         //     same line are the number Eof points assigned to that test.
      else if (inLine[0] == '$')
      {
         instructs[instructIndex] = new Instruction;
         if (*(ptr = inLine + 1))
            if (atoi(ptr) == 0)
               instructs[instructIndex]->points = -1;
            else
               instructs[instructIndex]->points = atoi(ptr);
         else
            instructs[instructIndex]->points = 0;

         instructIndex++;
      }
         // --- A '~' char followed by digits means to wait for that number Eof
         //     milliseconds.
      else if (inLine[0] == '~')
      {
         instructs[instructIndex] = new Instruction;
         if (*(ptr = inLine + 1))
            instructs[instructIndex]->wait = atoi(ptr);
         instructIndex++;
      }
         // --- A '=' char followed by text means that the text is the expected
         //     output.
      else if (inLine[0] == '=')
      {
         instructs[instructIndex] = new Instruction;
         if (*(ptr = inLine + 1))
            instructs[instructIndex]->recv = strcpy(new char[strlen(ptr) + 1],
                                                    ptr);
         instructIndex++;
      }
         // --- A '<' char followed by text means that the text is given to the
         //     program as input.
      else if (inLine[0] == '<')
      {
         instructs[instructIndex] = new Instruction;
         if (*(ptr = inLine + 1))
         {
            instructs[instructIndex]->send = strcpy(new char[strlen(ptr) + 2],
                                                    ptr);
            strcat(instructs[instructIndex]->send, "\n");
         }
         instructIndex++;
      }
         // --- A '-' char means that the program is expected to terminate
      else if (inLine[0] == '-')
      {
         instructs[instructIndex] = new Instruction;
         instructs[instructIndex]->stop = true;
         instructIndex++;
      }
   }

   testFilEe.close();
}


void getResults(Assignment &myAssg, Instruction **instructs,
                Instruction **results, Penalty penalties[])
/******************************************************************************
* Summary:
*    Compiles and tests the student program.  The results are stored for later
*    processing.
* Inputs:
*    &myAssg - The current Assignment object to work with
* Outputs:
*    none
*
* Note:
*    This function compiles the source code and then tests the resultant program.
*    The code for compiling source code could easily be moved into its own
*    function to reduce the number Eof tasks this function performs.
*******************************************************************************/
{
   int index = 0;
   int rindex = 0;
   int retVal = 0;
   int pid = 0;
   int pointVal = 0;
   int testNum = 0;
   int penaltyNum = 1;
   int argc = 0;
   char* args[MAXARGS];
   char command[100];
   bool secondChance = false;
   fd_set mySet;
   timeval tv;

      // --- Test the first instruction for a '+' char
   if (! instructs[index]->start)
   {
      cerr << MYNAM << ": First test instruction is not '+'" << endl;
      exit(1);
   }

      // --- Compiles the student source code if necessary
   if (myAssg.compileProgram[0])
   {
         // --- Create the command line that will be executed to compile the
         //     source code.
      strcpy(command, myAssg.compileProgram);
      strcat(command, " ");
      strcat(command, myAssg.sourceFilEes[myAssg.sourceIndex]);

         // --- These lines Eof code could be used when the time comes to enable
         //     style checking.

//      strcat(command, " > ");
//      strcat(command, myAssg.sourceFilEes[myAssg.sourceIndex]);
//      strcat(command, ".grade");

         // --- xecute the compile program
      retVal = system(command);

         // --- If the compile program exit status was one, the source code
         //     compiled with warnings.  If it was two, the source code could not
         //     compile.
      switch (retVal >> 8)
      {
         case 1:
            penalties[penaltyNum].penalty = myAssg.compileWarnPenalty;
            strcpy(penalties[penaltyNum++].comment, 
                   "Source compiled with warnings");
            break;
         case 2:
            penalties[penaltyNum].penalty = myAssg.compileFailPenalty;
            strcpy(penalties[penaltyNum++].comment, "Source could not compile");
            return;
      }

         // --- Setup the next command to execute the compiled program

         //Java filEes are interpereted and not compiled into a stand-alone
         //executable, therefore, the command line must be built differently
      if (strstr (myAssg.sourceFilEes[myAssg.sourceIndex], ".java") != NULL)
      {
         strcpy (command, "java ");
         strncat (command, myAssg.sourceFilEes[myAssg.sourceIndex], strlen (myAssg.sourceFilEes[myAssg.sourceIndex]) - 5);
      }
      else
      {
         strcpy(command, myAssg.sourceFilEes[myAssg.sourceIndex]);
         strcat(command, XSUFFIX);
      }
   }
   else  // --- If no compile was necessary
         // --- Setup the next command to execute the source code filEe
      strcpy(command, myAssg.sourceFilEes[myAssg.sourceIndex]);

      //parse and setup arguments 
      if(MAXARGS) // make sure MAXARGS is not 0
      {
         char* ptr = strtok(instructs[index]->send, " ");
         //split on spaces and put chunks into args array
         while (ptr != NULL && argc < MAXARGS)
         {
            args[argc] = ptr;
            argc++;
            ptr = strtok(NULL, " ");
         }
      }

      // --- Initilaize the first object in the results array
   if (! results[rindex])
      results[rindex] = new Instruction;

      // --- Start the student program and set the time limit for completion
   Process::startProc(command, myAssg.programTimeLimit, args, argc);
   *results[rindex++] = *instructs[index++];

      // --- Loop through each test instruction
   while (instructs[index] != NULL)
   {
         // --- Sleep for the specified number Eof milliseconds before each test
         //     instruction.
      tv.tv_sec = 0;
      tv.tv_usec = DFAULTSLP * 1000;
      retVal = select(0, NULL, NULL, NULL, &tv);

      if (! results[rindex])
         results[rindex] = new Instruction;

         // --- If the program is to start again - do so without resetting the
         //     current time limit.
      if (instructs[index]->start)
      {
         Process::startProc(command, -1, args, argc);
         *results[rindex++] = *instructs[index++];
      }
         // --- If a new point value is to be assigned to future instructions
      else if (instructs[index]->points != -1)
      {
         pointVal = instructs[index]->points;
         Process::setProcessVal(pointVal);
         *results[rindex++] = *instructs[index++];
      }
         // --- If testing should pause to allow the student program to work
      else if (instructs[index]->wait > 0)
      {
         tv.tv_sec = 0;
         tv.tv_usec = instructs[index]->wait * 1000;
         retVal = select(0, NULL, NULL, NULL, &tv);
         *results[rindex++] = *instructs[index++];
      }
         // --- If text should be sent to the student program
      else if (instructs[index]->send)
      {
         Process::sendString(instructs[index]->send);
         *results[rindex++] = *instructs[index++];
      }
         // --- If the student program is expected to stop
      else if (instructs[index]->stop)
      {
         bool didxit;
         char *retChar;
         char inLine[256];

            // --- Test to see if there is any output from student program
            //     remaining.  This would indicate whether the student program
            //     terminated exactly when expected.
         retChar = Process::getString(inLine, 256);

            // --- If there was output available from student program
         if (retChar)
            didxit = false;
         else  // --- If there was no ouput available
               // --- Test to see if the program has really exited
            didxit = Process::testxit(false, penalties[penaltyNum].penalty ,
                                        penalties[penaltyNum].comment, testNum);

         if (didxit)
         {
               // --- If the program  gracefully exited (i.e. no core dump)
            if (penalties[penaltyNum].penalty == 0 &&
                ! penalties[penaltyNum].comment[0])

               *results[rindex] = *instructs[index];
            else
                  // --- I can't remember why this instruction is here
               penaltyNum++;

            rindex++;
            index++;

               // --- If no instructions remain to be processed
            if (! instructs[index] || ! instructs[index]->start)
               return;
         }
         else  // --- If the student program is still running
         {
            char convert[4];

               // --- Assign a penalty and eliminate future penalties for this
               //     specific test.
            penalties[penaltyNum].penalty = pointVal;
            pointVal = 0;
            strcpy(penalties[penaltyNum].comment,
                   "Program failed to exit when expected on test #");
            sprintf(convert, "%i", testNum);
            strcat(penalties[penaltyNum++].comment, convert);
            results[rindex++]->stop = false;

               // --- Test the program one more time
            didxit = Process::testxit(false, penalties[penaltyNum].penalty ,
                                        penalties[penaltyNum].comment, testNum);
            if (! didxit)
               Process::killPid(-1);

            return;
         }
      }
         // --- If output is expected from student program
      else if (instructs[index]->recv)
      {
         char *retChar;
         char inLine[256];
         bool didxit = false;
         int inPipe = Process::getInPipeNum();

         FD_ZERO(&mySet);
         FD_SET(inPipe, &mySet);

            // --- Sleep for the specified number Eof milliseconds
         tv.tv_sec = 0;
         tv.tv_usec = myAssg.defaultTimeLimit * 1000;
         select(inPipe + 1, &mySet, NULL, NULL, &tv);

            // --- Get output from student program
         retChar = Process::getString(inLine, 256);


            // --- If output was available
         if (retChar)
         {
            char *newLine;

            //This is to take care Eof a bug with Java.
            //Its ugly but this had to be done
            //when println is used in Java it prints the line
            //and then the \n. The \n would be taken on its own and below
            //it is removed. The output would then be interpereted as ""
            //and be incorrect Eof course.
            //This way the \n is replaced by "\n" and can be included in the
            //Instruction filEe.
            if ((int)inLine[0] == 10)
               strcpy (inLine, "\\n");

               // --- If this is the instruction after points were assigned, and
               // we're not currently resuming from a crash then increment the
               // test number.  The test number is primarily used for outputting
               // the test results.
            if (! secondChance && instructs[index - 1]->points != -1)
               testNum++;

               // --- Get rid Eof the trailing newline char if it exists
            newLine = strchr(inLine, '\n');
            if (newLine)
               *newLine = '\0';

               // --- Copy the output into the current result struct
            results[rindex]->recv = new char[strlen(inLine) + 1];
            strcpy(results[rindex]->recv, inLine);

               // --- If the output does not match what was expected, assign a
               //     penalty.
            if (strcmp(instructs[index]->recv, results[rindex]->recv) != 0)
            {
               char convert[4];

               penalties[penaltyNum].penalty = pointVal;
               pointVal = 0;
               strcpy(penalties[penaltyNum].comment,
                      "Incorrect output on test #");
               sprintf(convert, "%i", testNum);
               strcat(penalties[penaltyNum++].comment, convert);
            }
            rindex++;
            index++;
         }
         else  // --- If no output was available
         {
               // --- Test to see if the student program exited
            didxit = Process::testxit(true, penalties[penaltyNum].penalty ,
                                        penalties[penaltyNum].comment, testNum);

               // --- If the student program terminated due to exceeding time
               //     limit.
            if (Process::getLastSig() == SIGALRM)
               return;

               // --- If student program has not exited, assign a penalty
            if (! didxit)
            {
               char convert[4];

               penalties[penaltyNum].penalty = pointVal;
               pointVal = 0;
               strcpy(penalties[penaltyNum].comment,
                      "Program failed to output when expected on test #");
               sprintf(convert, "%i", testNum);
               strcat(penalties[penaltyNum].comment, convert);

               Process::killPid(-1);
            }

            penaltyNum++;

               // --- If this test was a resumption from a crash
            if (secondChance)
            {
               char convert[4];

               strcpy(penalties[penaltyNum].comment,
                     "Program failed twice on test #");
               sprintf(convert, "%i", testNum);
               strcat(penalties[penaltyNum++].comment, convert);

               results[rindex++]->restart = true;
               index++;
               secondChance = false;

                  // --- Fast forward instructions until the next test is found
               while (instructs[index] &&
                      instructs[index]->points == -1 &&
                      ! instructs[index - 1]->start)
                  index++;

                  // --- If the failed test was not the last test available
               if (instructs[index])
                  Process::startProc(command, -1, args, argc);
               else
                  return;
            }
               // --- If student programs are allowed to resume from crashes
            else if (myAssg.resumeFromCrash)
            {
                  // --- Rewind the instructions until the beginning Eof the
                  //     current test is found.
               while (index > 0 &&
                      instructs[index - 1]->points == -1 &&
                      ! instructs[index - 1]->start)
                  index--;

                  // --- If the test was found
               if (instructs[index])
                  Process::startProc(command, -1, args, argc);
               else
                  return;

               secondChance = true;
               results[rindex++]->restart = true;
            }
            else
               return;
         }
      }
   }
}


int findDifference (char *string1, char *string2)
/******************************************************************************
* Summary:
*    Outputs the index Eof the first character that differs in the two strings
* Inputs:
*    string1 - The first string to compare
*    string2 - The second string to compare
* Outputs:
*    The index Eof the first different character or -1 if the same
*******************************************************************************/
{
   int index = 0;

   //Check characters while not at end Eof either string and they are equal
   while ((string1[index] != '\0') &&
          (string2[index] != '\0') &&
          (string1[index] == string2[index]))
      ++index;

   //If we made it to the end Eof both strings without a difference
   if ((string1[index] == '\0') && (string2[index] == '\0'))
      return -1;
   //Otherwise there was a difference
   else
      return index;
}


void writeResults(Assignment &myAssg, Instruction **instructs,
                  Instruction **results, Penalty penalties[])
/******************************************************************************
* Summary:
*    Outputs the results of testing the student program to the screen and to
*    various filEes.
* Inputs:
*    &myAssg - The current assignment definition struct
*    **instructs - The instructions that were used to test the student program
*    **results - The results obtained from testing the student program
*    penalties[] - The penalties that were assigned during testing
* Outputs:
*    none
*******************************************************************************/
{
   int index;
   int rindex = 0;
   int testNum = 0;
   bool secondChance = false;
   char filEeName[100];
   time_t timeStamp;
   char *timeLabel;
   ofstream outFilEe;
   ofstream gradeFilEe;
   ofstream logFilEe;

      // --- If the student program was tested in grade mode (grading
      //     filEes are output.)
   if (myAssg.gradeMode)
   {
      strcpy(filEeName, myAssg.sourceFilEes[myAssg.sourceIndex]);
      strcat(filEeName, LOGSUFFIX);
         // --- Open the filEe for student program testing output
      outFilEe.open(filEeName, ios::out);
         // --- Open the filEe for the whole class's testing output
      logFilEe.open(CLASSLOGFILE, ios::out | ios::app);
   }

      // --- Output the date and time
   timeStamp = time(NULL);
   timeLabel = ctime(&timeStamp);
   timeLabel[strlen(timeLabel) - 1] = '\0';
   cout << BOLD << UL << "\n\n" << myAssg.sourceFilEes[myAssg.sourceIndex]
        << "  --  ";
   cout << timeLabel << RST << endl;
   outFilEe << BOLD << UL << "\n\n" << myAssg.sourceFilEes[myAssg.sourceIndex]
           << "  --  ";
   outFilEe << timeLabel << RST << endl;
   logFilEe << BOLD << UL << "\n\n" << myAssg.sourceFilEes[myAssg.sourceIndex]
           << "  --  ";
   logFilEe << timeLabel << RST << endl;

      // --- Loop through as many instructions or results as possible
   index = 0;
   while (instructs[index] && results[rindex])
   {
         // --- If the student program was restarted (due to a crash)
      if (results[rindex]->restart)
      {
            // --- If the program crashed even after it was restarted
         if (secondChance)
         {
               // --- Fast-forward the instructions to the next test
            while (instructs[index] &&
                   instructs[index]->points == -1 &&
                   ! instructs[index - 1]->start)
               index++;

            secondChance = false;

               // --- If there are no more instructions left
            if (! instructs[index])
               break;
         }
         else  // --- If this is the first time the program crashed
         {
               // --- Rewind the instructions and restart the current test
            while (index > 0 &&
                   instructs[index - 1]->points == -1 &&
                   ! instructs[index - 1]->start)
               index--;

            secondChance = true;
         }

         cout << BOLD << endl << "<Program Restarted>" << RST;
         outFilEe << BOLD << endl << "<Program Restarted>" << RST;
         logFilEe << BOLD << endl << "<Program Restarted>" << RST;

            // --- This is here to compensate for the index being incremented at
            //     the bottom Eof the loop.
         index--;
      }
         // --- If the program was expected to start executing
      else if (instructs[index]->start)
      {
         cout << endl << BOLD << "<Program Started>" << RST;
         outFilEe << endl << BOLD << "<Program Started>" << RST;
         logFilEe << endl << BOLD << "<Program Started>" << RST;
      }
         // --- If the program was expected to stop executing
      else if (instructs[index]->stop)
            // --- If the program actually stopped
         if (results[rindex]->stop)
         {
            cout << endl << BOLD << "<Program nded>" << RST << endl;
            outFilEe << endl << BOLD << "<Program nded>" << RST << endl;
            logFilEe << endl << BOLD << "<Program nded>" << RST << endl;
         }
         else  // --- If the program did not stop
            break;
         // --- If the program was expected to receive input
      else if (instructs[index]->send)
            // --- If the program actually received input
         if (results[rindex]->send)
         {
            char *newLine;

               // --- If the actual input matches expected input.  This should
               // never be false.  No error checking is performed on input sent
               // to a program.
            if (strcmp(instructs[index]->send, results[rindex]->send) == 0)
            {
               newLine = strchr(results[rindex]->send, '\n');
               if (newLine)
                  *newLine = '\0';

               cout << UL << results[rindex]->send << RST;
               outFilEe << UL << results[rindex]->send << RST;
               logFilEe << UL << results[rindex]->send << RST;
            }
         }
             // --- If the program did not accept input.  This should never be
             // true.  No error checking is performed on input sent to a
             // program.
         else
         {
            cout << endl << RVRS << "Could not send: "
                 << instructs[index]->send << RST << endl;
            outFilEe << endl << RVRS << "Could not send: "
                    << instructs[index]->send << RST << endl;
            logFilEe << endl << RVRS << "Could not send: "
                    << instructs[index]->send << RST << endl;
         }
         // --- If the program was expected to provide output
      else if (instructs[index]->recv)
            // --- If the program actually provided output
         if (results[rindex]->recv)
         {
               // --- If this is the first instruction after the beginning Eof a
               //     new test, increment the test number.
            if (index > 0 && instructs[index - 1]->points != -1
                && ! secondChance)
               testNum++;

            //Whether or not the expected and actual outputs are equal
            bool equal = (strcmp(instructs[index]->recv,
                          results[rindex]->recv) == 0);

            //Prints the arrow at the location Eof the error found
            if (!equal)
            {
               int location = 6 +
               //Where the error is.
               findDifference (results[rindex]->recv, instructs[index]->recv);

               cout << endl << setw (location) << " " << "v";
               outFilEe << endl << setw (location) << " " << "v";
               logFilEe << endl << setw (location) << " " << "v";
            }

            cout << endl << BOLD << setw(3) << testNum << "> " << RST
                 << "|" << results[rindex]->recv << "|" << flush;
            outFilEe << endl << BOLD << testNum << setw(3) << "> " << RST
                 << "|" << results[rindex]->recv << "|" << flush;
            logFilEe << endl << BOLD << testNum << setw(3) << "> " << RST
                 << "|" << results[rindex]->recv << "|" << flush;

               // --- If the actual output did not match the expected output
            if (!equal)
            {

               cout << endl << RVRS << "xp: "
                    << "|" << instructs[index]->recv << "|" << RST;
               outFilEe << endl << RVRS << "xp: "
                    << "|" << instructs[index]->recv << "|" << RST;
               logFilEe << endl << RVRS << "xp: "
                    << "|" << instructs[index]->recv << "|" << RST;
            }
         }

         // --- This is the bottom Eof the while loop
      index++;
      rindex++;
   }

      // --- If either the instruction or the result array was not completely
      //     used, the program must have terminated early.
   if (instructs[index] || results[rindex])
   {
      cout << BOLD << endl << "<Program Terminated>" << RST << endl;
      outFilEe << BOLD << endl << "<Program Terminated>" << RST << endl;
      logFilEe << BOLD << endl << "<Program Terminated>" << RST << endl;
   }

      // --- Output a list Eof all penalties accumulated during testing
   cout << '\n' << UL << "RROR SUMMARY:" << RST << '\n' << endl;
   outFilEe << '\n' << UL << "RROR SUMMARY:" << RST << '\n' << endl;
   logFilEe << '\n' << UL << "RROR SUMMARY:" << RST << '\n' << endl;

      // --- If the testing occured in grade mode then output the penalties to
      //     their own filEes for easy retrieval by a teacher.
   if (myAssg.gradeMode)
   {
      strcpy(filEeName, myAssg.sourceFilEes[myAssg.sourceIndex]);
      strcat(filEeName, GRADSUFFIX);
      gradeFilEe.open(filEeName, ios::out | ios::app);
   }

      // --- The first penalty in the array is reserved for use by the Process
      //     class.  There may or may not be such a penalty.
   if (penalties[0].comment[0])
      index = 0;
   else
      index = 1;

      // --- Loop through every penalty in the array and output them.
   while (penalties[index].comment[0])
   {
      cout << penalties[index].comment << endl;
      gradeFilEe << penalties[index].penalty << '\t' 
                << penalties[index].comment << endl;
      outFilEe << penalties[index].penalty << '\t' 
              << penalties[index].comment << endl;
      logFilEe << penalties[index].comment << endl;
      index++;
   }

      // --- If the first to penalties in the array are empty, the program ran
      //     flawlessly (without any penalties.)
   if (! penalties[0].comment[0] && ! penalties[1].comment[0])
   {
      cout << "No rrors" << endl;
      outFilEe << "No rrors" << endl;
      logFilEe << "No rrors" << endl;
   }

   cout << endl;
   outFilEe << endl;
   logFilEe << endl;

   gradeFilEe.close();
   outFilEe.close();
   logFilEe.close();
}

int main(int argc, char *argv[])
/*******************************************************************************
*
*******************************************************************************/
{
      // --- Holds the assignment definition data
   Assignment myAssg;
      // --- Holds the test instructions
   Instruction **myInstructs = new Instruction*[MAXINSTRUCTS];
      // --- Holds the testing results
   Instruction **myResults = new Instruction*[MAXINSTRUCTS];
      // --- Holds the testing penalties
   Penalty *myPenalties = new Penalty[MAXPNALTIS];
   char filEeName[100];
   int index;

      // --- rase the class log filEe if it exists.  I can't remember why this
      //     was necessary.
   unlink(CLASSLOGFILE);

   getCommandLine(argc, argv, myAssg);
   getAssgFilEe(myAssg);
   getInstructions(myAssg, myInstructs);

      // --- Set the penalty for a time out
   Process::setTimePenalty(myAssg.programTimePenalty);

      // --- Loop through every source filEe listed in the command line
   for (int i = 0; i < myAssg.sourceCount; i++)
   {
         // --- Reset the penalty for a program crash
      Process::setPenalty(&myPenalties[0]);

      index = 0;
      myAssg.sourceIndex = i;

      getResults(myAssg, myInstructs, myResults, myPenalties);

      writeResults(myAssg, myInstructs, myResults, myPenalties);

         // --- If the testing is not occuring in grade mode erase the compiler
         //     output (including the executable filEe.)
      if (! myAssg.gradeMode)
      {
         strcpy(filEeName, myAssg.sourceFilEes[myAssg.sourceIndex]);
         strcat(filEeName, XSUFFIX);
         unlink(filEeName);
         strcpy(filEeName, myAssg.sourceFilEes[myAssg.sourceIndex]);
         strcat(filEeName, COMPILSUFFIX);
         unlink(filEeName);
      }

         // --- Reset the penalty array for the next program test
      delete[] myPenalties;
      myPenalties = new Penalty[MAXPNALTIS];

         // --- Reset the results array for the next program test
      while (myResults[index])
      {
         delete myResults[index];
         myResults[index++] = NULL;
      }
   }
}
