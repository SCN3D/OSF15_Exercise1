#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


#include "matrix.h"


#define MAX_CMD_COUNT 50

/*protected functions*/
void load_matrix (Matrix_t* m, unsigned int* data);

/* 
 * PURPOSE: instantiates a new matrix with the passed name, rows, cols 
 * INPUTS: 
 *	name the name of the matrix limited to 50 characters 
 *  rows the number of rows the matrix
 *  cols the number of cols the matrix
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/

bool create_matrix (Matrix_t** new_matrix, const char* name, const unsigned int rows,
						const unsigned int cols) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!name){
		printf("name is not valide\n");
		return false;
	}
	if(rows <= 0 ){
		printf("rows is not valide\n");
		return false;
	}
	if(cols <= 0){
		printf("cols is not valide\n");
		return false;
	}

	*new_matrix = calloc(1,sizeof(Matrix_t));
	if (!(*new_matrix)) {
		return false;
	}
	(*new_matrix)->data = calloc(rows * cols,sizeof(unsigned int));
	if (!(*new_matrix)->data) {
		return false;
	}
	(*new_matrix)->rows = rows;
	(*new_matrix)->cols = cols;
	unsigned int len = strlen(name) + 1; 
	if (len > MATRIX_NAME_LEN) {
		return false;
	}
	strncpy((*new_matrix)->name,name,len);
	return true;

}

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: free the memory of matrix list
 * INPUTS: 
 * m the matrix list
 * RETURN: void 
 * If no errors occurred during destroy then return nothing
 * else print error message
 **/
void destroy_matrix (Matrix_t** m) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!(*m)){
		printf("matrix is null\n");
		return;
	}
	free((*m)->data);
	free(*m);
	*m = NULL;
}


	
	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: compare two matrix to check if it is equal
 * INPUTS: 
 * a the matrix  to be compare
 * b the matrix to be compared
 * RETURN:
 *  If no errors occurred during compare and the two matrix are equal then true
 *  else false.
 *
 **/
bool equal_matrices (Matrix_t* a, Matrix_t* b) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	
	if (!a || !b || !a->data || !b->data) {
		return false;	
	}

	int result = memcmp(a->data,b->data, sizeof(unsigned int) * a->rows * a->cols);
	if (result == 0) {
		return true;
	}
	return false;
}

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: copy the scource matrix to the destination memory
 * INPUTS: 
 * src the matrix  to be copyed
 * dest the destination matrix 
 * RETURN:
 *  If no errors occurred during duplicate then true
 *  else false for an error in the process.
 *
 **/
bool duplicate_matrix (Matrix_t* src, Matrix_t* dest) {


	//TODO ERROR CHECK INCOMING PARAMETERS

	if (!src) {
		printf("source matrix is null\n");
		return false;
	}
	if(!dest){
		printf("dest matrix is null\n");
		return false;
	}
	/*
	 * copy over data
	 */
	unsigned int bytesToCopy = sizeof(unsigned int) * src->rows * src->cols;
	memcpy(dest->data,src->data, bytesToCopy);	
	return equal_matrices (src,dest);
}

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: bit shift a matrix with given direction and shift number
 * INPUTS: 
 * a the matrix to bit shift
 * direction the direction of shift, left or right
 * shift the number to shift 
 * RETURN:
 *  If no errors occurred during shift then true
 *  else false for an error in the process.
 *
 **/
bool bitwise_shift_matrix (Matrix_t* a, char direction, unsigned int shift) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS
	if (!a) {
		return false;
	}
	if(direction != 'l' || direction != 'r'){
		printf("Wrong direction!\n");
		return false;
	}

	if (direction == 'l') {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] << shift;
			}
		}

	}
	else {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] >> shift;
			}
		}
	}
	
	return true;
}

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: combine first two matrix to third matrix
 * INPUTS: 
 * a the first matrix
 * b the second matrix
 * c the third matrix
 * RETURN:
 *  If no errors occurred during process then true
 *  else false for an error in the process.
 *
 **/
bool add_matrices (Matrix_t* a, Matrix_t* b, Matrix_t* c) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!a || !b){
		printf("there is/are null matrix\n");
		return false;
	}

	if (a->rows != b->rows && a->cols != b->cols) {
		return false;
	}

	for (int i = 0; i < a->rows; ++i) {
		for (int j = 0; j < b->cols; ++j) {
			c->data[i * a->cols +j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
		}
	}
	return true;
}

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: display the contents in given matrix
 * INPUTS: 
 * m the matrix to display
 * RETURN: void 
 * If no errors occurred during print then return nothing
 * else print error message
 **/
void display_matrix (Matrix_t* m) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!m){
		printf("matrix is empty\n");
		return;
	}

	printf("\nMatrix Contents (%s):\n", m->name);
	printf("DIM = (%u,%u)\n", m->rows, m->cols);
	for (int i = 0; i < m->rows; ++i) {
		for (int j = 0; j < m->cols; ++j) {
			printf("%u ", m->data[i * m->cols + j]);
		}
		printf("\n");
	}
	printf("\n");

}

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: read a file with given name and create a new matrix list
 * INPUTS: 
 * matrix_input_filename the file name to be read
 * m the matrix list to create
 * RETURN:
 *  If no errors occurred during read and create then true
 *  else false for an error in the process.
 *
 **/
bool read_matrix (const char* matrix_input_filename, Matrix_t** m) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!matrix_input_filename){
		printf("Invalide file name\n");
		return false;
	}

	int fd = open(matrix_input_filename,O_RDONLY);
	if (fd < 0) {
		printf("FAILED TO OPEN FOR READING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	/*read the wrote dimensions and name length*/
	unsigned int name_len = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;
	
	if (read(fd,&name_len,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READING FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	char name_buffer[50];
	if (read (fd,name_buffer,sizeof(char) * name_len) != sizeof(char) * name_len) {
		printf("FAILED TO READ MATRIX NAME\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;	
	}

	if (read (fd,&rows, sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX ROW SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read(fd,&cols,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX COLUMN SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	unsigned int numberOfDataBytes = rows * cols * sizeof(unsigned int);
	unsigned int *data = calloc(rows * cols, sizeof(unsigned int));
	if (read(fd,data,numberOfDataBytes) != numberOfDataBytes) {
		printf("FAILED TO READ MATRIX DATA\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;	
	}

	if (!create_matrix(m,name_buffer,rows,cols)) {
		return false;
	}

	load_matrix(*m,data);
	free(data);
	if (close(fd)) {
		return false;

	}
	return true;
}

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: write the given matrix into a file
 * INPUTS: 
 * matrix_output_filename the file name to be wrote
 * m the matrix to write
 * RETURN:
 *  If no errors occurred during write then true
 *  else false for an error in the process.
 *
 **/
bool write_matrix (const char* matrix_output_filename, Matrix_t* m) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!m){
		printf("Given matrix is empty/invalide\n");
		return false;
	}

	int fd = open (matrix_output_filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	/* ERROR HANDLING USING errorno*/
	if (fd < 0) {
		printf("FAILED TO CREATE/OPEN FILE FOR WRITING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXISTS\n");
		}
		return false;
	}
	/* Calculate the needed buffer for our matrix */
	unsigned int name_len = strlen(m->name) + 1;
	unsigned int numberOfBytes = sizeof(unsigned int) + (sizeof(unsigned int)  * 2) + name_len + sizeof(unsigned int) * m->rows * m->cols + 1;
	/* Allocate the output_buffer in bytes
	 * IMPORTANT TO UNDERSTAND THIS WAY OF MOVING MEMORY
	 */
	unsigned char* output_buffer = calloc(numberOfBytes,sizeof(unsigned char));
	unsigned int offset = 0;
	memcpy(&output_buffer[offset], &name_len, sizeof(unsigned int)); // IMPORTANT C FUNCTION TO KNOW
	offset += sizeof(unsigned int);	
	memcpy(&output_buffer[offset], m->name,name_len);
	offset += name_len;
	memcpy(&output_buffer[offset],&m->rows,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset],&m->cols,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy (&output_buffer[offset],m->data,m->rows * m->cols * sizeof(unsigned int));
	offset += (m->rows * m->cols * sizeof(unsigned int));
	output_buffer[numberOfBytes - 1] = EOF;

	if (write(fd,output_buffer,numberOfBytes) != numberOfBytes) {
		printf("FAILED TO WRITE MATRIX TO FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	
	if (close(fd)) {
		return false;
	}
	free(output_buffer);

	return true;
}

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: create a matrix with random numbers and given range
 * INPUTS: 
 * m the matrix to randomize
 * start_range the min number of random
 * end_range the max number of random
 * RETURN:
 *  If no errors occurred during process then true
 *  else false for an error in the process.
 *
 **/
bool random_matrix(Matrix_t* m, unsigned int start_range, unsigned int end_range) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!m){
		printf("Matrix is null\n");
		return false;
	}
	if(start_range >= end_range){
		printf("Invalide range\n");
		return false;
	}

	for (unsigned int i = 0; i < m->rows; ++i) {
		for (unsigned int j = 0; j < m->cols; ++j) {
			m->data[i * m->cols + j] = rand() % (end_range + 1 - start_range) + start_range;
		}
	}
	return true;
}

/*Protected Functions in C*/

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: load data into given matrix
 * INPUTS: 
 * m the matrix create
 * data the data to load
 * RETURN: void
 *  If no errors occurred during process then nothing
 *  else terminate function
 *
 **/
void load_matrix (Matrix_t* m, unsigned int* data) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!m){
		printf("Matrix is null\n");
		return;
	}
	if(!data){
		printf("Invalide data\n");
		return;
	}
	memcpy(m->data,data,m->rows * m->cols * sizeof(unsigned int));
}

	//TODO FUNCTION COMMENT
/* 
 * PURPOSE: add given matrix to a new matrix array
 * INPUTS: 
 * mats the matrix list
 * new_matrix the matrix to be added
 * num_mats the index of matrix in the list
 * RETURN: 
 *  If no errors occurred during process then return the position pos
 *  else return 999
 **/
unsigned int add_matrix_to_array (Matrix_t** mats, Matrix_t* new_matrix, unsigned int num_mats) {
	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!new_matrix){
		printf("new_matrix is null\n");
		return 999;
	}
	if(!(*mats)){
		printf("matrix list is null\n");
		return 999;
	}
	if(num_mats < 0){
		printf("Invalide number\n");
		return 999;
	}


	static long int current_position = 0;
	const long int pos = current_position % num_mats;
	if ( mats[pos] ) {
		destroy_matrix(&mats[pos]);
	} 
	mats[pos] = new_matrix;
	current_position++;
	return pos;
}
