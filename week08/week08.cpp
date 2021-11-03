
/*********************************************************************
*   CSE 453
*
*   Carlos N Reina
*   Adrian Whetten
*   Jordan Burdett
*   Caleb Georgeson
*   Russell Roberts
*   Stephen Ellis
*   Orion Christensen
*
*********************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

/*************************************
 * ARRAY VULNERABILTY
 * 1. There must be an array and an array index variable
 * 2. The array index variable must be reachable through external input.
 * 3. There must not be bounds checking on the array index variable.
 ****************************************/
void arrayVulnerability(/* feel free to add parameters */)
{
}

/**************************************
 * ARRAY WORKING
 * Call arrayVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void arrayWorking()
{
   arrayVulnerability(/* parameters with non-malicious data */);
}

/**************************************
 * ARRAY EXPLOIT
 * 1. The attacker provides an array index value outside the expected range
 * 2. The attacker must be able to provide input or redirect
 *    existing input into the array at the index he provided
 * 3. The injected value must alter program state in a way
 *    that is desirable to the attacker
 *************************************/
void arrayExploit()
{
   arrayVulnerability(/* parameters with malicious data */);
}


// ARC Injection

/**************************************
 * arcVulnerability
 * 
 *************************************/
void arcVulnerability()
{
   
}

/**************************************
 * arcWorking
 * 
 *************************************/
void arcWorking()
{
   
}

/**************************************
 * arcExploit
 * 
 *************************************/
void arcExploit()
{
   
}

/**********************************************
 * MAIN : 
 **********************************************/
int main()
{
    return 0;
}
