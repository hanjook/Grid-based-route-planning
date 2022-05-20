#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
	int row;
	int col;
	int counter;
	struct node* next;
} node_t;

typedef struct grids {
	char grid;
	int counter_grid;
	int check_flag;
} grids_t;



int mygetchar();

void gridsize();
grids_t** make_the_grid();
int place_the_contents();
void print_the_grid();

void print_stage();

node_t* make_the_route();
void print_list();
void new_node();

int status_check();
void repair_route();

int check_if_connects();
node_t* make_way_backwards();
void place_the_path();
void link_the_nodes();

void free_list();
void free_grid();

int
main(int argc, char *argv[]) {
	
int vert, hori;
int blockcount;
grids_t** grids;
node_t* head;	

print_stage(0);
gridsize(&vert, &hori);
grids = make_the_grid(vert, hori, &grids);
blockcount = place_the_contents(grids);

head = make_the_route(&head);
place_the_path(head, grids, 0);
if(status_check(head, grids, vert, hori, blockcount) == 0) {
	print_stage(1);
	place_the_path(head, grids);
	print_the_grid(vert, hori, grids);
}
	print_stage(3);
	free_list(head);
	free_grid(grids, vert);

return 0;
}

/*----------------------------------------------------*/
int
mygetchar() {
	/* function used specifically for this assignment due to difference in
		operating systems */
	int c;
	while ((c = getchar())=='\r') {
	}
	return c;
}


/*----------------------------------------------------*/
void
gridsize(int* vert, int* hori){
	/* scans the stdin for gridsizes */ 
	
	scanf("%dx%d", vert, hori);
	printf("The grid has %d rows and %d columns.\n", *vert, *hori);
}


/*----------------------------------------------------*/
grids_t**
make_the_grid(int vert, int hori, grids_t** grids) {
	int i,j;
	/* function that dynamically allocates memory for the struct to be used as
		as grid */
	
	/* dynamically allocate memory to the 'grids' struct and make a 
		2d array of 'grids' struct */
	
	grids = (grids_t**)malloc(vert * sizeof(grids_t*));
	assert(grids != NULL);
	for(i = 0; i < 10; i++) {
		grids[i] = (grids_t*)malloc(hori * sizeof(grids_t));
		assert(grids[i] != NULL);
	
	}
	
	for(i = 0; i < vert; i++) {
		for(j = 0; j < hori; j++) {
			/* assign whitespaces to the grid to be printed out, make every
				flag of the grid 0, and every counter to -1 */
			
			grids[i][j].grid = ' ';
			grids[i][j].check_flag = 0;
			grids[i][j].counter_grid = -1;
		}
	}
	
	
	return grids;
}


/*----------------------------------------------------*/
int
place_the_contents(grids_t** grids) {
	
	/* function that inserts the contents from the scanned stdin into the grid
		*/
	
	int inity, initx, goaly, goalx;
	int y, x;
	int blockcount = 0;
	
	/* inserts the initial cell and goal cell into the grid */
	scanf(" [%d,%d] ", &inity, &initx);
	grids[inity][initx].grid = 'I';
	
	scanf(" [%d,%d] ", &goaly, &goalx);
	grids[goaly][goalx].grid = 'G';

	
	while(scanf(" [%d,%d] ", &y, &x) == 2) {
		/* inserts the blocks into the grid and gives it properties so that
			it is later not overwrited by other functions*/
		blockcount += 1;
		grids[y][x].grid = '#';
		grids[y][x].counter_grid = -3;
		grids[y][x].check_flag = 1;
	}
	
	printf("The grid has %d block(s).\n", blockcount);
	printf("The initial cell in the grid is [%d,%d].\n", inity, initx);
	printf("The goal cell in the grid is [%d,%d].\n", goaly, goalx);
	mygetchar();
	return blockcount;
}


/*----------------------------------------------------*/
void
print_the_grid(int vert, int hori, grids_t** grids) {
	
	/* visualize the grid in the format needed */
	
	int y, x;
	
	printf(" ");
	for(x = 0; x < hori; x++) {
		/* print the x-axis */
		printf("%d", x);
	}
	printf("\n");
	for(y = 0; y < vert; y++) {
		/*print the y-axis*/
		printf("%d", y);
		for(x = 0; x < hori; x++) {
			/*print the contents of the grid */
			printf("%c", grids[y][x].grid);
		}
	printf("\n");
	}
}



/*----------------------------------------------------*/
node_t*
make_the_route(node_t* head) {
	
	/* make a linked list with the coordinates scanned from the stdin */
	
	int x, y;
	
	/* allocate memory to the head of the linked list and add information
		of the first coordinate scanned */
	
	head = (node_t *)malloc(sizeof(node_t));
	assert(head!= NULL);
	scanf(" [%d,%d]->", &y, &x);
	head->row = y;
	head->col = x;
	head->counter = 0;
	head->next = NULL;
	
	while(scanf(" [%d,%d]->", &y, &x) == 2) {
		/* add new coordinates onto the end of the linked list */
		
		new_node(head, y, x, 0);
	}
	
	printf("The proposed route in the grid is:\n");
	print_list(head);
	
	return head;
}

/*----------------------------------------------------*/
void
new_node(node_t* start, int y, int x, int count) {
	node_t* new_cell;
	node_t* foot = start;
	
	/* allocate memory for the new cell of the linked list */
	
	new_cell = (node_t *)malloc(sizeof(node_t));
	assert(new_cell != NULL);
	new_cell->row = y;
	new_cell->col = x;
	new_cell->counter = count;
	new_cell->next = NULL;
	
	
	while(foot->next != NULL) {
		
		/* go to the end of the list */
		
		foot = foot->next;
	}
	foot->next = new_cell;
}

/*----------------------------------------------------*/
void
print_list(node_t* head) {
	int cellcount = 0;
	node_t* current = head;
	while(1) {
		if(current->next != NULL) {
			
			/* keep going through the linked list while printing them out */
			
			if(cellcount < 5) {
				
				/* while the cell is not at the end, print them out 5 cells
					at a time with an arrow */
				
				printf("[%d,%d]->", current->row, current->col);
				current = current->next;
				cellcount += 1;
			}
			
			else {
				printf("\n");
				cellcount = 0;
			}
		}
		
		else {
			
			/* if you have reached the end of the linked list, print the 
				cell out with a full stop */
			
			printf("[%d,%d].", current->row, current->col);
			break;
		}
	}
	printf("\n");
}

/*----------------------------------------------------*/
int
status_check(node_t* head, grids_t** grids, int vert, int hori, \
				int blockcount) {
	/* function that checks the status of the route out of the 5 provided */
	
	int x, y, flag = 0;
	node_t* current = head; 
	
	
	while(current->next != NULL) {
		
		/* current will be used below to find the coordinate of the goal */
		
		current = current->next;
	}
	
	if(grids[head->row][head->col].grid != 'I') {
		
		/* check if the corresponding coordinates in the grid is the initial 
			cell 'I' */
		
		printf("Initial cell in the route is wrong!\n");
		flag = 1;
	}
	else if(grids[current->row][current->col].grid != 'G') {
		
		/* check if the corresponding coordinates in the grid is the goal cell
			'G' */
		
		printf("Goal cell in the route is wrong!\n");
		flag = 1;
		/* flag indicating error */
	}
	
	current = head;
	y = current->row;
	x = current->col;
	current = current->next;
	/* store the coordinates of the cell before the current one */
	
	while(current != NULL) {
		if(abs((current->row)-y) > 1 || abs((current->col)-x) > 1) {
			printf("There is an illegal move in this route!\n");
			flag = 1;
			/* flag indicating error */
		}
		/* check if the cell moved more than one cell or diagonally */
		
		else if((current->row) >= vert || (current->col) >= hori || \
				(current->row) < 0 || (current->col) < 0) {
				/* check if the cell moved out of the grid */
				
				printf("There is an illegal move in this route!\n");
				
				flag = 1;
				/* flag indicating error */
		}
		
		y = current->row;
		x = current->col;
		current = current->next;
		/* store the coordinates of the current cell and move on to the next
			one */
	}
	
	current = head;
	/* point back to the head cell */
	
	while(current != NULL) {
		/* again, go through the linked list, this time checking for blocks */
		
		if(grids[current->row][current->col].grid == '#') {
			/* check the grid's coordinates to see if there is a block */
			printf("There is a block on this route!\n");
			print_stage(1);
			repair_route(current, head, grids, vert, hori, blockcount);
			/* if so, start repairing */
			
			flag = 1;
			/* flag indicating error */
			
		}
		current = current->next;
	}
	current = head;
	
	if(flag == 0) {
		/* if nothing happened, the route is valid! */
		printf("The route is valid!\n");
	}
	return flag;
}

/*----------------------------------------------------*/

void
repair_route(node_t* block, node_t* head, grids_t** grids, \
				int vert, int hori, int blockcount) {

	node_t* head_dummy; 
	/* a dummy that points back to the head cell */
	
	node_t* start = malloc(sizeof(node_t));
	assert(start != NULL);
	/* let the start be the cell of the break of the original route
		(start of where the route will begin to repair) 
		the linked list made beginning from this cell will be referred
		to as the 'bridge' */
		
	node_t* start_dummy;
	/* a dummy that points back to the start cell */
	node_t* link;
	
	node_t* new_head = malloc(sizeof(node_t));
	assert(new_head != NULL);
	new_head->row = head->row;
	new_head->col = head->col;
	new_head->next = NULL;
	/* will be used as new route pointer */
	
	
	int count = 0;
	int y, x;
	int limit = 1;
	
	head_dummy = head;
	
	while(head_dummy->next != block) {
		/* go through the linked lists again until you encounter the block */
		
		head_dummy = head_dummy->next;
	}
	
	
	
	start->row = head_dummy->row;
	start->col = head_dummy->col;
	start->counter = 0;
	start->next = NULL;
	
	start_dummy = start;
	head_dummy = head;
	
	grids[start->row][start->col].counter_grid = 0;
	grids[start->row][start->col].check_flag = 1;
	
	while(grids[block->row][block->col].grid == '#') {
		/* now, iterate from the block to see where the broken route starts
			again */
		block = block->next;
	}
	
	link = block;
	/* this is the cell that the new route can possibly link to */
	
	
	while(limit < vert*hori - blockcount - 2) {
		/* this loop checks above, below, left and right of the cell in
			that order, and checks whether it is a link back to the 
			original route */
		/* conditions are thatwhile the grid has not been fully filled yet,
			keep finding a link to the original path 2 is from the two cells: 
			initial and goal cell */

		
			y = start->row;
			x = start->col;			
			count = start->counter;
			count += 1;
			
			if(y-1 >= 0) {
				/* checking for the cell above the current one */
				
				if(grids[y-1][x].check_flag != 1) {
					new_node(start, y-1, x, count);
					/* append the cell to the 'start' linked list */
					
					grids[y-1][x].counter_grid = count;
					/* append the counter to the grid */
					
					if(check_if_connects(y-1, x, link, grids) == 1) {
						y -=1;
						break;
					}
					grids[y-1][x].check_flag = 1;
					limit+= 1;
				}
			}
			
			/* everything is repetitive except it checks in different
				directions */
				
			if(y+1 < vert) {
				/* checking for the cell below the current one */
				
				if(grids[y+1][x].check_flag != 1) {
					new_node(start, y+1, x, count);
					grids[y+1][x].counter_grid = count;
					if(check_if_connects(y+1, x, link, grids) == 1) {
						y += 1;
						break;
					}
					grids[y+1][x].check_flag = 1;
					limit+= 1;
				}
			}
			
			if(x-1 >= 0) {
				/* checking for the cell left of the current one */
				
				if(grids[y][x-1].check_flag != 1) {
					new_node(start, y, x-1, count);
					grids[y][x-1].counter_grid = count;
					if(check_if_connects(y, x-1, link, grids) == 1) {
						x -=1;
						break;
					}
					grids[y][x-1].check_flag = 1;
					limit+= 1;
				}
			}
			
			if(x+1 < hori) {
				/* checking for the cell right of the current one */
				
				if(grids[y][x+1].check_flag != 1) {
					new_node(start, y, x+1, count);
					grids[y][x+1].counter_grid = count;
					if(check_if_connects(y, x+1, link, grids) == 1) {
						x +=1;
						break;
					}
					grids[y][x+1].check_flag = 1;
					limit+= 1;
				}
			}
			
			start = start->next;
		}
		
	start = start_dummy;
	
	link_the_nodes(y, x, head_dummy, head, new_head, link, start, grids);
	print_the_grid(vert, hori, grids);
	printf("------------------------------------------------\n");
	place_the_path(new_head, grids, 1);
	print_the_grid(vert, hori, grids);
	printf("------------------------------------------------\n");
	print_list(new_head);
	status_check(new_head, grids, vert, hori, blockcount);
	
	free_list(new_head);
	
	}


/*---------------------------------------------------------*/
int
check_if_connects(int y, int x, node_t* link, grids_t** grids) {
	/* function that checks if the cells that makes up the repaired route
		meets a possible link when repairing */
	
	node_t* check = link;
	while(check != NULL) {
		if(check->row == y && check->col == x) {
			while(check != NULL) {
				/* check if the possible link cell has blocks in it's
					pathway. If there is, it cannot be used as a link
					to the original route */
				if(grids[check->row][check->col].grid == '#') {
					return 0;
					/* indicates that it cannot be used as a link */
				}
				check = check->next;
			}
		return 1;
		/* indicates that it can be used as a pathway */
		}
		check = check->next;
	}
	if(grids[y][x].grid == 'G') {
		/* if the possible link cell is actually our goal cell, then it can 
			also be used as a link cell */
			
			return 1;
	}
	return 0;
}


	
/*---------------------------------------------------------*/
void
link_the_nodes(int y, int x, node_t* head_dummy, node_t* head, \
				node_t* new_head, node_t* link, node_t* start, grids_t** grids) 
				{

	node_t* new_repair = malloc(sizeof(node_t));
	assert(new_repair != NULL);
	
	
	node_t* new_link = malloc(sizeof(node_t));
	assert(new_link != NULL);
	
	node_t* new_repair_dummy = new_repair;

	/* make new nodes which will be linked together to make the new repaired
		route */
	
	while(head_dummy != NULL) {
		
		if(head_dummy->row == y && head_dummy->col == x) {
			new_repair->row = head_dummy->row;
			new_repair->col = head_dummy->col;
			new_repair->next = NULL;
			new_repair_dummy = new_repair;
			break;
		}
		/* makes the new_repair point to the values of at 
			link to work backwards */
		head_dummy = head_dummy->next;
	}
	
	head_dummy = head_dummy->next;
	new_link->row = head_dummy->row;
	new_link->col = head_dummy->col;
	new_link->next = NULL;
	/* makes the new_link point to the values of the node after link */
	
	link = head_dummy->next;
	while(link!= NULL) {
		new_node(new_link, link->row, link->col, link->counter);
		link = link->next;
	}
	/* copy the route that the that was the original route after the link
		was found */
	
	head_dummy = head;
	head_dummy = head_dummy->next;
	while(head_dummy != start) {
		/* copy the route from the beginning now, and stop when it meets
		the 'start' cell */
		new_node(new_head, head_dummy->row, head_dummy->col, 0);
		if(head_dummy->next->row == start->row \
			&& head_dummy->next->col == start->col) {
			break;
		}
		head_dummy = head_dummy->next;
	}
	
	
	while(1) {
		/* similarly to before it checks above, below, left then right
			in preference when backtracking the route back to the 
			'start' cell all the while making the new route 'new_repair' */
		if(new_repair->row == start->row && new_repair->col == start->col) {
			break;
		}
		if(grids[y-1][x].counter_grid == grids[y][x].counter_grid - 1) {
			/* when it encounters a grid cell with a counter one below 
				itself */
			y -= 1;
			new_repair = make_way_backwards(y, x, new_repair);
			continue;
		}
		if(grids[y+1][x].counter_grid == grids[y][x].counter_grid - 1) {
			y += 1;
			new_repair = make_way_backwards(y, x, new_repair);
			continue;
		}
		if(grids[y][x-1].counter_grid == grids[y][x].counter_grid - 1) {
			x -= 1;
			new_repair = make_way_backwards(y, x, new_repair);
			continue;
		}
		if(grids[y][x+1].counter_grid == grids[y][x].counter_grid - 1) {
			x += 1;
			new_repair = make_way_backwards(y, x, new_repair);
			continue;
		}
	}
	
	
	/* now link the linked_lists together */
	
	while(new_head->next != NULL) {
		new_head = new_head->next;
	}
	new_head->next = new_repair;
	/* new head to the new_repair */
	while(new_head->next != NULL) {
		new_head = new_head->next;
	}
	
	new_head->next = new_link;
	/* new_head to the new_link */
	
	free_list(start);
	
	/* now it points to the start */
}
	

	

/*---------------------------------------------------------*/
node_t*
make_way_backwards(int y, int x, node_t* new_repair) {
	/* make your way back from the link cell to the 'start' by adding a 
		new cell at the beginning of new_repair. (backtracking)*/
	
	node_t* backtrack;
	backtrack = malloc(sizeof(node_t));
	assert(backtrack != NULL);
	
	backtrack->row = y;
	backtrack->col = x;
	backtrack->counter = 0;
	backtrack->next = new_repair;
	
	new_repair = backtrack;
	
	return new_repair;
}

/*---------------------------------------------------------*/
void
place_the_path(node_t* head, grids_t** grids) {
	/* a function to place the path inside the grid */
	node_t* head_dummy;
	head_dummy = head;
	
	while(head_dummy != NULL) {
		/* in order to exclude the last cell(the goal cell) */
		head_dummy = head_dummy->next;
	}
	head = head->next;
	/* in order to exclude the first cell (the initial cell) */
	while(head->next != head_dummy) {
		/* put the asterisks in the grid */
		if(grids[(head->row)][(head->col)].grid == ' ') {
			grids[(head->row)][(head->col)].grid = '*';
		}
		head = head->next;
	}
}

/*---------------------------------------------------------*/
void
print_stage(int stage) {
	/* to print the layout */
	
	if(stage == 0 || stage == 1 || stage == 2) {
		printf("==STAGE %d=======================================\n", stage);
	}
	else {
		printf("================================================\n");
	}
	}
	
	
/*---------------------------------------------------------*/	
void
free_list(node_t* list) {
	/* function from chapter 10, listops.c */
	node_t* prev;
	assert(list != NULL);
	while (list != NULL) {
		prev = list;
		list = list->next;
		free(prev);
		prev = NULL;
	}
}

/*---------------------------------------------------------*/
void
free_grid(grids_t** grids, int vert) {
	
	/* to free the grid's memory */
	
	int i;
	for(i = 0; i < vert; i++) {
		free(grids[i]);
		grids[i] = NULL;
	}
	free(grids);
	grids = NULL;
}







