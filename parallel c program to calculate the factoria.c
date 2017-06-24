
#include <stdio.h>
#include "mpi.h"

int c=0 ;
int main(int argc, char *argv[]){
    int myRank;
    MPI_Status status;
    int tag = 0;
   // double begin;
    double local_result = 1.0;
    double total;
    int size;
    int fact;
    int lower,upper;
    int i;
    double total_fact=1 ;
    float start1=0.0,end1=0.0;
    float start2=0.0,end2=0.0;
float time1=5.0 ;

    /* initialize MPI */
    MPI_Init(&argc,&argv);
   

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* get the input if rank =0 */

    if(myRank==0){
	printf("Please Enter a number:");
  start1 = mpi_wtime() ;
	scanf("%d",&fact);


    }
  
       MPI_Bcast(&fact, 1,MPI_INT,0,MPI_COMM_WORLD);

  
    /* calculate the upper and lower boundaries
     * for each process
     */
    if(myRank==0){
	lower = 1;
    }else
	lower = myRank * (fact / size) + 1;
    if(myRank==(size-1)) 
	upper = fact; /*if the factorial is even or odd the (size-1) process will handle it  */
    else
	upper = (myRank + 1) * (fact / size);

    /*now we will multiply the local area 
     */
    for(i=lower;i<=upper;i++){
	local_result = local_result * (double)i;
    }
   
    printf("process %d  calculate partial factorial from %d to %d = %lf \n",myRank,lower,upper,local_result);

        
      

    MPI_Reduce(&local_result, &total,1,MPI_DOUBLE, MPI_PROD,0,MPI_COMM_WORLD);
      
 
    /* print the output */

    if(myRank==0){
	printf("The factorial of %d = %lf,\nand was calculated using %d processes\n",fact,total,size);
    


    
    }



   if(myRank==0)
    {
start2== mpi_wtime () ;
        printf("\n----------send and receive-----------\n");

     for ( i = 1; i < (size); ++i)
    {
        
            MPI_Send(&fact, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
    }   

    int source ;
    double local_recv ;
        for(source = 1; source < size; source++)
        {
           MPI_Recv(&local_recv, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status );
           total_fact = total_fact*local_recv ;
        }
end2=mpi_wtime() ;
        float time2 = start2 - end2 ;
        printf("The factorial (send / receive) of %d = %lf, \nand was calculated using %d processes\n",fact,total_fact,size-1);

        printf(" \n----------run time compare------------\n" );
        printf("Runtime using (send / receive) =%f \n",time2 );

    }
    else
    {
        
        MPI_Recv(&fact, 1, MPI_INT, 0, tag, MPI_COMM_WORLD , &status );
       // printf("test %d rank %d \n", (fact / (size-1)),myRank);
        lower = ((myRank- 1 )* (fact / (size-1))) + 1;
       // printf("lower %d\n", lower);
       if(myRank==(size-1)) 
         upper = fact; /*if the factorial is even or odd the (size-1) process will handle it  */
    else
         upper = (myRank) * (fact / (size-1));

    /*now we will multiply the local area 
     */
     int j ;
     local_result=1.0 ;
    for(j=lower;j<=upper;j++){
    local_result = local_result * (double)j;
    }
        printf("process %d  calculate partial factorial from %d to %d = %lf \n",myRank,lower,upper,local_result);
    MPI_Send(&local_result, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
       
    }
    end1 = mpi_wtime () ;
      time1 = start1 - end1   ;
       printf("Runtime using reduce/bcast= %f \n", time1);



    MPI_Finalize();



    return 0;
}
