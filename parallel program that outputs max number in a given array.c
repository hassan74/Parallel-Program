#include<stdio.h>
#include <stdlib.h>
#include "mpi.h"

 int * assignWork(int *arr ,int (*arr2) ,int index ,int arr2size)
 {
     int i=index ;
     int j=0 ;
     for( ;i<arr2size ;i++)
     {
         arr2[j]=arr[i] ;
         j++ ;
     }
     return arr2 ;
 }

int main(int argc , char * argv[])
{
    int rank;
    int nprocess;
    int source;
    int dest;
    int tag = 0;
    int *arr ;
    int *arr2 ;
    int arr2size ;
    int blockSize;
    int subSum = 0;
    int maxElement;
    int indexMax ;
    int totalMax= -100000 ;
    int indexTotalMax ;
    MPI_Status status;

    /* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, & nprocess);

    if( rank == 0) /** Master work **/
	{
	    int ArraySize, i ;

	    printf("Enter array size: ");
	    scanf("%d", &ArraySize);
          
	    arr = (int*) malloc(ArraySize * sizeof(int));

          
        printf("Please enter array elements …: ");
	    for(i=0; i< ArraySize; i++)
           scanf("%d", &arr[i]);
        
         arr2size = ArraySize / (nprocess-1);

          arr2=(int*) malloc(arr2size * sizeof(int));
          int index=0;
        for(dest = 1; dest < nprocess ; dest++)
        {
           if(dest==(nprocess-1) && (ArraySize % (nprocess-1))!=0)
           {
            
            int newArrayize=arr2size+(ArraySize % (nprocess-1));
            MPI_Send(&newArrayize, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            MPI_Send( &arr[index] , newArrayize, MPI_INT, dest, tag, MPI_COMM_WORLD);

           }
           else
           {
            MPI_Send(&arr2size, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            MPI_Send( &arr[index] , arr2size, MPI_INT, dest, tag, MPI_COMM_WORLD);
            index+=arr2size;
           }

        }
        for(source = 1; source < nprocess; source++)
        {
           MPI_Recv(&maxElement, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status );
           MPI_Recv(&indexMax, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status );
           printf("hello from slave# %d Max number in my partition is %d and index is %d \n", source ,maxElement ,indexMax);
           if(maxElement >totalMax)
           {
            totalMax=maxElement ;
            indexTotalMax =((source-1)*arr2size)+indexMax ;//to get index of the final max
           }
        }

        printf("Master process announce the final max which is= %d and its index is %d \n", totalMax ,indexTotalMax);

	}
	else /** Slave work **/
	{
	    source = 0;

	    MPI_Recv(&arr2size, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status );
         arr2=(int*) malloc(arr2size * sizeof(int));
	    MPI_Recv(arr2, arr2size, MPI_INT, source, tag, MPI_COMM_WORLD, &status );

	    int i;
        indexMax =0 ; 
        maxElement=arr2[0] ;
        //get max of the block

 for(i=0; i<arr2size; i++)
            {
                printf("element %d rank  %d\n",arr2[i] ,rank);
               
            }
	    for(i=1; i<arr2size; i++)
            {
             
                if(arr2[i]>maxElement)
                {
                    maxElement=arr2[i] ;
                    indexMax=i ;
                }
            }
        dest = 0;
        MPI_Send(&maxElement, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        MPI_Send(&indexMax, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}

	/* shutdown MPI */
	MPI_Finalize();
    return 0;
}
