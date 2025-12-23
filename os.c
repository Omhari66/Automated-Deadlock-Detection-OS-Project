#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* ---------- Clear Input Buffer ---------- */
void clearInputBuffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// ---------- Helper: Allocate 2D Matrix ----------
int **allocateMatrix(int rows, int cols)
{
    int **mat = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        mat[i] = (int *)malloc(cols * sizeof(int));
    }
    return mat;
}

// ---------- Input Module ----------
bool inputData(int *n, int *r, int ***allocation, int ***request, int **available)
{
    printf("Enter number of processes: ");
    if (scanf("%d", n) != 1 || *n <= 0)
    {
        printf("ERROR: Number of processes must be positive.\n");
          clearInputBuffer();
        return false;
    }

    printf("Enter number of resource types: ");
    if (scanf("%d", r) != 1 || *r <= 0)
    {
        printf("ERROR: Number of resource types must be positive.\n");
          clearInputBuffer();
        return false;
    }

    *allocation = allocateMatrix(*n, *r);
    *request = allocateMatrix(*n, *r);
    *available = (int *)malloc(*r * sizeof(int));

    printf("\nEnter Allocation Matrix:\n");
    for (int i = 0; i < *n; i++){
        printf("P%d: ",i);
        for (int j = 0; j < *r; j++){
           if(scanf("%d", &(*allocation)[i][j])!=1)
           {
            printf("Invalid input in Allocation Matrix\n");
                clearInputBuffer();
                return false;
           }
        }
    }
    printf("\nEnter Request Matrix:\n");
    for (int i = 0; i < *n; i++){
    printf("P%d: ",i);
        for (int j = 0; j < *r; j++){
            if(scanf("%d", &(*request)[i][j])!=1){
                 printf("Invalid input in Request Matrix\n");
                clearInputBuffer();
                return false;
            }
        }
    }

    printf("\nEnter Available Resources:\n");
    for (int j = 0; j < *r; j++)
       if(scanf("%d", &(*available)[j])!=1){
         printf("Invalid input in Request Matrix\n");
                clearInputBuffer();
                return false;
       }
    

    return true;
}

/*
---------------------------------------------------------------------------
Deadlock Detection Algorithm (C Implementation)
// 
The algorithm follows these steps:
1. Copy the Available[] array into a temporary Work[] array.
2. Initialize all processes as unfinished (finish[i] = false).
3. Search for a process whose outstanding Request[i][j] <= Work[j] for all j.
4. If found, simulate completion: add its Allocation back to Work and mark it finished.
5. Repeat until no more processes can be marked finished.
6. Any process that remains unfinished is part of the deadlock set.

Returns: number of deadlocked processes.
Fills the deadlocked[] array with process IDs of deadlocked processes.
---------------------------------------------------------------------------

*/


// ---------- Deadlock Detection Module ----------
// Returns the number of deadlocked processes.
// Fills the 'deadlocked' array with PIDs of deadlocked processes.
int detectDeadlock(int n, int r, int **allocation, int **request,
     int *available, int *deadlocked)
{
      // Step 1: Create Work[] = Available[]
    int *work = (int *)malloc(r * sizeof(int));
    for (int i = 0; i < r; i++)
        work[i] = available[i];
// Step 2: Create Finish[] and initialize to false (0)
    bool *finish = (bool *)malloc(n * sizeof(bool));
    for (int i = 0; i < n; i++)
        finish[i] = false;
 // Step 3 & 4: Try to find a process that can finish
    bool progress;
    do
    {
        progress = false;
        for (int i = 0; i < n; i++)
        {
            if (!finish[i])// Process not finished yet
            {
                bool canFinish = true;
                for (int j = 0; j < r; j++)
                {
                    if (request[i][j] > work[j])
                    {
                        canFinish = false;
                        break;
                    }
                }
                // If process can finish, release its resources to Work
                if (canFinish)
                {
                    for (int j = 0; j < r; j++)
                        work[j] += allocation[i][j];
                    finish[i] = true;
                    progress = true;
                }
            }
        }
    } while (progress);// Repeat until no more processes can finish

// Step 5: Collect processes that are still unfinished (deadlocked)
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (!finish[i])
        {
            deadlocked[count++] = i;
        }
    }

 // Free temporary memory
    free(work);
    free(finish);
    return count; // Return number of deadlocked processes
}

// ---------------------------------------------------------------------------
// Recovery Algorithm
// When a process is aborted:
// - All its allocated resources are returned to Available[]
// - Its allocation row becomes 0
// - Its request row becomes 0 (process no longer needs anything)
// ---------------------------------------------------------------------------

// ---------- Recovery Module ----------
// ---------------------------------------------------------------------------
// applyRecovery()
// This function simulates aborting a process during deadlock recovery.
// Steps:
// 1. The process is "terminated" (simulated).
// 2. All resources allocated to that process are released back to 'available'.
// 3. Allocation and Request entries for that process are cleared to 0.
// ---------------------------------------------------------------------------
void applyRecovery(int pid, int r, int **allocation, int **request, int *available)
{
    printf("\n[Recovery] Aborting Process P%d...\n", pid);

    for (int j = 0; j < r; j++)
    {
        available[j] += allocation[pid][j];  // Release allocated resources
        allocation[pid][j] = 0;              // Clear allocation
        request[pid][j] = 0;                 // Clear pending request
    }

    printf("[Recovery] Process P%d aborted. Resources released.\n", pid);
}


// ---------- System Display ----------
void printSystem(int n, int r, int **allocation, int **request, int *available)
{
    printf("\nAllocation Matrix:\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d: ", i);
        for (int j = 0; j < r; j++)
            printf("%d ", allocation[i][j]);
        printf("\n");
    }

    printf("\nRequest Matrix:\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d: ", i);
        for (int j = 0; j < r; j++)
            printf("%d ", request[i][j]);
        printf("\n");
    }

    printf("\nAvailable Resources: ");
    for (int j = 0; j < r; j++)
        printf("%d ", available[j]);
    printf("\n");
}

// ---------- MAIN ----------
int main()
{
    int runAgain = 1;

    while (runAgain)
    {
        printf("============================================\n");
        printf("       AUTOMATED DEADLOCK DETECTION TOOL    \n");
        printf("============================================\n\n");

        int n, r;
        int **allocation = NULL;
        int **request = NULL;
        int *available = NULL;

        /* Step 1: Input */
        if (!inputData(&n, &r, &allocation, &request, &available))
        {
            printf("\nInput Error. Restarting...\n\n");
            continue;
        }

        /* Step 2: Display system */
        printSystem(n, r, allocation, request, available);

        /* Step 3: Detect deadlock */
        int *deadlocked = (int *)malloc(n * sizeof(int));
        int deadlockedCount = detectDeadlock(
            n, r, allocation, request, available, deadlocked);

        printf("\n============================================\n");
        printf("                 RESULTS                    \n");
        printf("============================================\n\n");

        if (deadlockedCount == 0)
        {
            printf("SAFE STATE - No Deadlock Detected.\n");
        }
        else
        {
            printf("DEADLOCK DETECTED!\n");
            printf("Deadlocked Processes: ");
            for (int i = 0; i < deadlockedCount; i++)
                printf("P%d ", deadlocked[i]);
            printf("\n");

            /* Step 4: Recovery (OLD DESIGN PRESERVED) */
            while (1)
            {
                printf("\nRecovery Options:\n");
                printf("1. Abort a process\n");
                printf("2. Show system state\n");
                printf("3. Exit recovery\n");
                printf("Enter choice: ");

                int choice;
                if(scanf("%d", &choice)!=1){
                    printf("Invalid input in Request Matrix\n");
                    clearInputBuffer();
                    return false;
                }

                if (choice == 3)
                {
                    printf("Exiting recovery.\n");
                    break;
                }
                else if (choice == 2)
                {
                    printSystem(n, r, allocation, request, available);
                }
                else if (choice == 1)
                {
                    int pid;
                    printf("Enter process ID to abort: ");
                    if(scanf("%d", &pid)!=1){
                         printf("Invalid input in Request Matrix\n");
                        clearInputBuffer();
                        return false;
                    }

                    if (pid < 0 || pid >= n)
                    {
                        printf("Invalid PID.\n");
                        continue;
                    }

                    applyRecovery(pid, r, allocation, request, available);

                    deadlockedCount = detectDeadlock(
                        n, r, allocation, request, available, deadlocked);

                    if (deadlockedCount == 0)
                    {
                        printf("\nSYSTEM RECOVERED - No Deadlock Now.\n");
                        break;
                    }
                    else
                    {
                        printf("\nStill Deadlocked. Remaining: ");
                        for (int i = 0; i < deadlockedCount; i++)
                            printf("P%d ", deadlocked[i]);
                        printf("\n");
                    }
                }
                else
                {
                    printf("Invalid choice.\n");
                }
            }
        }

        printf("\n============================================\n");
        printf("                   END                      \n");
        printf("============================================\n");

        /* Cleanup */
        free(deadlocked);
        free(available);
        for (int i = 0; i < n; i++)
        {
            free(allocation[i]);
            free(request[i]);
        }
        free(allocation);
        free(request);

        printf("\nDo you want to run the tool again? (1 = Yes, 0 = No): ");
        scanf("%d", &runAgain);
        printf("\n");
    }

    printf("Program terminated successfully.\n");
    return 0;
}
