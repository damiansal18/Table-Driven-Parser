
#include<iostream>
#include<stdlib.h>
#include<fstream>
//#include"stack.h"  // ** can be removed
#include<vector>
#include<string>
#include <stdio.h>
#include <ctype.h>
#include <stack>
using namespace std;

//---------------------------------------
// CS421 File ll1.cpp for HW3A LL1 Table-Driven Parser
// Your name: Damian Salazar
//---------------------------------------

// Complete this to fit the HW3A specification - look for **
// Must have the same types of tracing couts as my demo program.

// Feel free to use a stack class or use a vector
// to implement your stack.  The stack will contain characters.

vector<char> M[3][2];  // the table of rules 
                       // 3 rows for S, A, B
                       // 2 rows for 0, 1
       // Each slot contains a rule's right side
       // which is a vector of characters

//  ------- conversion functions ------------------------


// to convert non-terms S, A, B to table rows 0, 1, 2
int toRow(char C)
{
  //**  bunch of if then else
  if (C == 'S')
	  return 0;
  if ( C == 'A') 
	  return 1;
  if ( C == 'B')
	  return 2;

  return 0;
}

// to convert '0' and '1' to table columns 0 and 1 
int toCol(char c)
{
  // ** bunch of if then else
  if (c == '0')
	  return 0;
  if (c == '1')
	  return 1;
  // default
  return 0;
}

// to convert row 0, 1, 2 to non-terms S, A and B
char toNonterm(int r)
{
  // ** bunch of if then else
  if (r == 0)
	  return 'S';
  if (r == 1)
	  return 'A';
  if (r == 2)
	  return 'B';
	
  	//default
	return 'A';
}



// ** Then complete the following functions.-----------------------------

// to display a rule's rhs which is in vector V
void displayVector(vector<char> V) 
{
  // ** display V horizontally e.g. 0 A 0
  for (int i = 0; i < V.size(); i++)
  {
	  cout << V[i] << " " ;
  } // end of for-loop

} // end of displayVector function

// to read in the rules into M, make sure ; is not stored
void readrules()
{ 
  int row, col;
  char row_c, col_c;  
  ifstream fin ("rules", ios::in);

  // For each line of "rules" (e.g. S  0 A 0 ;) 

  while( fin >> row_c )
  {
  	  // The first char of a line determines the row of M	  
	  // it outputs the characters into row number
	  row = toRow(row_c);

	  // The second char of a line determine the column of M
	  // it outputs the characters into col number
	  fin >> col_c;
	  col = toCol(col_c);
	  vector<char> v;
	  char c;
	  v.push_back(col_c);
	  while ( fin >> c)
	  {
		// Note that each entry of M (the second char up to ;)
		// will be a vector representing
		// the right hand side of a rule (rhs)
		// if statement declines any integer value from the text file.
		if ( c != ';')
		{
			v.push_back(c);
		}
		if( c == ';')
		{
			break;
		} 
	  }
	  
	M[row][col]=v;
  }
  fin.close();
  

  // ** Display the table nicely  
  int R = 3;
  int C = 2;
  for (int i = 0; i < R; i++)
  {
	  //    use toNonterm to show row labels (S, A, B)
	  char c = toNonterm(i);
	  cout << c << ":\t";
	  for (int j = 0; j < C; j++)
	  {
		  //    use displayVector for each content
		  displayVector(M[i][j]);
		  cout << "\t" ;
	  }
	  cout << endl;
  }

}// end of readrules Function

//  pushes V contents to the stack 
stack<char> addtostack(stack<char> V, vector<char> N)
{  cout << "Pushing rhs of a rule to the stack." << endl;
  // **  be careful of the order
   // because 1 0 0 means 1 will be at the top of the stack
	for (int i = N.size(); i >= 0; i--)
	{
		// reject any blank spaces
			if (N[i] != '\0')
			{
				// push the vector inside the stack
				V.push(N[i]);
			}
	}

	return V;
}// end of addtostack Function


void displayAll(stack<char> &V)
{
     // ** display the stack vertically from top to bottom
     if (V.empty())
     { 
	     // If stack is empty then return
	     //cout << "[ empty ]" << endl;
	     return;
     } else {
	        char x = V.top();
	     	// Pop the top element of the stack
		V.pop();
		// Print the stack element starting 
		// from the bottom
		cout << x << endl;
		displayAll(V);
		// Push the same element onto the stack
		// to preserve the order
		V.push(x);
	} // end of if statement

}// end of displayAll Function

// isTerminal Function
// PURPOSE: If true the character is the same as the non terminal
// PARAMETERS: one character
bool isTerminal(char ref)
{
	if (ref == 'A' || ref == 'B' || ref == 'S')
		return true;
	else 
		return false;
}// end of isTerminal Function

int main()
{ 
  readrules();  // M is filled and displayed 
  
  string ss;
  cout << "Enter a string made of 0's and/or 1's: ";
  cin >> ss;

  // ** push 'S' onto the stack to start
  stack<char> V;
  V.push('S'); 
  // ** display the stack vertically from top to bottom
  cout << "Stack:" << endl;
  displayAll(V);
  cout << "--------------" << endl; 
  int i = 0;  // index for ss
  cout << "current char is: " << ss[i] << endl;
  int charLength = ss.length();
  while (ss[i] != '\0')  // for each char of ss
  { 
     // Based on ss[i] and 
     //    the top of stack, update the stack:
      	    char ref = V.top();
	    // pop the front of the vector
	   
	     	   
	    // if is nonterminal pop the element from V stack
	    // and push vector into the V stack
	    if (isTerminal(ref))
	    {
		    V.pop();
		    int r = toRow(ref);
		    int c = toCol(ss[i]);

		    
		    // ** otherwise, addtoStack the M entry based on ss[i] and the top of stack 	
		    V = addtostack(V,M[r][c]);
		    // if the input and the V stack are not the same 
		    // stop the program
		    if( V.top() != ss[i])
		    {
			    cout << "current char is: " << ss[i] << endl;
			    cout << ">>Error - no rule. Reject!" << endl;
			    break;
		    }
		    // display the stack
		    cout << "Stack:" << endl;
                    displayAll(V);
                    cout << "--------------" << endl;
                    cout << "current char is: " << ss[i] << endl;
		   
	    } 
 	    
		
	    // if the input and V stack are the same
	    if (V.top() == ss[i])
	    {
		// pop the element from the V stack
		V.pop();
		// if the V stack is emtpy, and the total length of input char 
		// is the finalize, stop the program and it is Accept
		if (V.size() == 0 && charLength == i+1)
		{
			cout << "Stack:" << endl;
			cout << "[ empty ]" << endl;
			cout << "--------------" << endl;
			cout << ">>Accept!" << endl;
			break;
		}
		// if the V stack is empty, but the total length is greather than
		// current input character length, then stop the program and
		// it is rejected!
		if (V.size() == 0 && charLength > i+1)
		{
			// ** note that empty stack will cause immediate failure
			cout << "Stack:" << endl;
			displayAll(V);
			cout << "--------------" << endl;
			cout << ">>Error - Stack is empty. Reject!" << endl;
			break;
		}
		// display the V stack
		cout << "Stack:" << endl;
                displayAll(V);
                cout << "--------------" << endl;
		cout << "current char is: " << ss[i] << endl; 
                cout << "matched!" << endl;
		

	    }
	    // if is mistmatched stop the program, it is rejected!
	    else if( V.top() != ss[i])
	       {
		      // ** note that top of stack terminal and ss[i] mismatching
		      //    will cause immediate failure 
		       cout << ">>Mismatch Error - no rule. Reject!" << endl;
	    	break;
	    }
		    
    	     i++;
    } // end of string



}// end of main
