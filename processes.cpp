//---------------------------------------------------------------------------
// --------------------------- processes.cpp------------------------------------
// Angie(Nomingerel) Tserenjav
// CSS 430 Section A
// Creation Date: 08/10/19
// Date of Last Modification: 13/10/19
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// Purpose: This program is to familiarize with Linux programming using 
// several system calls such as fork, execlp, wait, pipe, dup2 and close. 
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// Notes: This is the .cpp file for processes.cpp that receives one arguments
// upon its invocation and searches how many processes whose name is given in 
// argv[1] are running on the system. ps -A | grep argv[1] | wc -l.
// --------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include <stdio.h>
#include <string>
#include <unistd.h>
#include <memory.h>
#include <sys/wait.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

enum { RD, WR };
char* in;
// this pipe is to connect between great-grand-child and grand-child
int pipefd1[2];
// this pipe is to connect between grand-child and child
int pipefd2[2];

// -----------------------------gGrandChild()-----------------------------
//gGrandchild() closes the pipefd2 [RD] which is great grand child and child, pipefd2[WR] 
//which is great-grand-child and child. Dup2 is makes the copy of the file discriptor and 
//execlp is executes the "ps -A" command. 

void gGrandChild() {
	close(pipefd2[RD]);
	close(pipefd2[WR]);
	close(pipefd1[RD]);
	dup2(pipefd1[WR], STDOUT_FILENO);
	execlp("ps", "ps", "-A", NULL);
}

/// -----------------------------gChild()-----------------------------
//gchild() closes the pipefd1 [WR] which is the WR great grand child and grand child, pipefd2[RD] 
//which is great-grand-child and child's read end. Dup2 is makes the copy of the file discriptor for RD 
//end of great grand child and grand child, other dup2 is WR of great grand child and grand child and 
//execlp is executes the "grep" command. 
void gChild() {
	close(pipefd1[WR]);
	close(pipefd2[RD]);
	dup2(pipefd1[RD], STDIN_FILENO);
	dup2(pipefd2[WR], STDOUT_FILENO);
	execlp("grep", "grep", in, NULL);
}

// -----------------------------child()-----------------------------
//child() closes the pipefd1 [WR] which is great grand child and child, pipefd1[RD] 
//which is great-grand-child and child. It also closes the pipefd2[WR] which is WR of gradn child 
//and child.Dup2 is makes the copy of the file discriptor for pipefd2[RD] which is grandchild and child. 
//Execlp is executes the "wc -l" command. 
void child() {
	close(pipefd1[WR]);
	close(pipefd1[RD]);
	close(pipefd2[WR]);
	dup2(pipefd2[RD], STDIN_FILENO);
	execlp("wc", "wc", "-l", NULL);
}

int main(int argc, char** argv) {

	int pid; //declare pid
	int st; //declare st, ending of the cur program

	//set the argument to 1
	in = argv[1];

	//argument check
	if (argc < 2) {
		cerr << "Error: Arguments not sufficient" << endl;
		return 0;
	}
	pid = fork();
	//pipe for great-grand-child, grand-child and grandchild, child
	if (pipe(pipefd2) < 0 || pipe(pipefd1) < 0) {
		cerr << "Error: Pipe failed." << endl;
	}
	if (pid < 0) {
		cerr < "Error: Fork failed." << endl;
	}

	if (pid == 0) { //great-grand-child

		pid = fork(); // create a child process
		if (pid == 0) { //grandchild

			pid = fork();
			if (pid == 0) { //child
				child();
			}
			else { 
				gChild();
			}
		}
		else { 
			gGrandChild();
		}
	}
	else {
		wait(&st); // wait for child processes
	}
	return 0;
}