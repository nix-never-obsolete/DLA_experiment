#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>






static int try_move(int direction, int on_x, int on_y, int width, int height,  unsigned char  grid[height][width]);

void write_header(FILE *file, int width, int height);





static int try_move(int direction, int on_x, int on_y, int width, int height, unsigned char grid[height][width])
{
	if(grid[on_y][on_x] >= 1){return(3);} 			// spawned inside other block.
							

	if(direction == 0) 					// going left
	{
		if(on_x <= 1){return(2);} 			// at left margin -> retry
		else if(grid[on_y][on_x-1] != 0){return(1);} 	// going into existing block -> landed
		else{return(0);} 				// free -> continue
	}


	else if(direction == 1) 				// going up
	{
		if(on_y <= 1){return(2);} 			// at upper margin -> retry
		else if(grid[on_y-1][on_x] != 0){return(1);} 	// going into existing block -> landed
		else{return(0);} 				// free -> continue
	}


	else if(direction == 2) 				// going right
	{
		if(on_x >= width-1){return(2);} 		// at right margin -> retry
		else if(grid[on_y][on_x+1] != 0){return(1);} 	// going into existing block -> landed
		else{return(0);} 				// free -> continue
	}


	else if(direction == 3) 				// going down
	{
		if(on_y >= height-1){return(2);} 		// at lower margin -> retry
		else if(grid[on_y+1][on_x] != 0){return(1);} 	// going into existing block -> landed
		else{return(0);} 				// free -> continue
	}


	else{return(4);} 					//something went wrong.
}








void write_header(FILE *file, int width, int height)
{	
	int filesize = (width*height*3 + height*((4-((width*3)%4)))+54);
	
	unsigned char header[54];
	
	
	header[0]=((unsigned char)('B'));
	header[1]=((unsigned char)('M'));
		
	header[2]=((unsigned char)(filesize));
	header[3]=((unsigned char)(filesize>> 8));
	header[4]=((unsigned char)(filesize>>16));
	header[5]=((unsigned char)(filesize>>24));
	
	header[6]=(0);
	header[7]=(0);
	header[8]=(0);
	header[9]=(0);
	header[10]=(54);
	header[11]=(0);
	header[12]=(0);
	header[13]=(0); 
	header[14]=(40);
	header[15]=(0);
	header[16]=(0);
	header[17]=(0); 
	
	header[18]=(((unsigned char)(width    )));
	header[19]=(((unsigned char)(width>> 8)));
	header[20]=(((unsigned char)(width>>16)));
	header[21]=(((unsigned char)(width>>24)));

	header[22]=(((unsigned char)(height    )));
	header[23]=(((unsigned char)(height>> 8)));
	header[24]=(((unsigned char)(height>>16)));
	header[25]=(((unsigned char)(height>>24))); 
	
	header[26]=(1);
	header[27]=(0);
	header[28]=(24);
	header[29]=(0);

	header[30]=(0);
	header[31]=(0);
	header[32]=(0);
	header[33]=(0);
        header[34]=(0);
	header[35]=(0);
	header[36]=(0);
	header[37]=(0);
        header[38]=(0);
	header[39]=(0);
	header[40]=(0);
	header[41]=(0);
        header[42]=(0);
	header[43]=(0);
	header[44]=(0);
	header[45]=(0);
        header[46]=(0);
	header[47]=(0);
	header[48]=(0);
	header[49]=(0);
        header[50]=(0);
	header[51]=(0);
	header[52]=(0);
	header[53]=(0);


				// it *really* need to revise this, 
				// but for now, it is functional.
				// it *is* really bad tho...



	for(int byte = 0; byte < 54; byte++)
	{
		fwrite(&header[byte], 1, 1, file);
	}


}








int main()
{
	srand(time(NULL)); 	
	
	int width;
	int height;
	int points;
	int starting_points;

	int on_rx; 				// x location of the marker
	int on_ry; 				// y location of the marker

	int ret; 				// return from try_move()

	int direction; 				// direction to try to move

	bool place; 				// has block been placed?
	

						// Using chars so it doesn't break after a few thousand.

	int r = 255;				// start red at max (will be faded out later)
	int g = 255;				// start blue at max (will be faded out later)
	int b = 255;				// start green at max (will be faded out later)
	


	printf("%s", "Width: ");			// ask for width
	scanf("%i", &width);				// set width from input
	
	printf("%s", "\nHeight: ");			// ask for height
	scanf("%i", &height); 				// set height from input
	
	printf("%s", "\nNumber of points: ");		// ask for number of points
	scanf("%i", &points);	 			// points to be added to the map
	
	printf("%s", "\nNumber of starting points: ");	// ask for starting/seed points
	scanf("%i", &starting_points);			// starting/seed points




	unsigned char grid[height][width]; 		// grid



	for(int y = 0; y < height; y++) 		// itterate through the grid and set values to 0.
	{						// i thought this wouldn't be necesairy, and that it
		for( int x = 0; x < width; x++) 	// would be initialized to 0s, but i kept getting
		{ 					// what appeared to be int overflows or junk data
			grid[y][x] = 0; 		// or something when I didn't have this, so who knows.
		}					// probably not the most efficient way, but compared to
	}						// the time to actually run the simulation, basically nothing.



	for (int starting_point = 0; starting_point < starting_points; starting_point++){
		grid[(rand() % (height-2))+1][(rand() % (width-2))+1] = 255;			// place the first blocks on the grid
	}



	for(int point = 0; point < points; point++)			// start the main loop
	{
		printf("%d%s", (point*100)/points, "\%\n");		// print out the percent finished

		on_rx = rand() % width; 				// randomize the marker's x position
		on_ry = rand() % height; 				// randomize the marker's y position

		place = false; 						// this point has not been placed
		
		
		while(!place)						// loop until the point is placed
		{							// (place == true)
			
			direction = rand() % 4;
			
			ret = try_move(direction,			// try to move and
				       on_rx,				// save the result
				       on_ry,				// ret().
				       width,
				       height,
				       grid
		      	);


			if(ret == 0)
			{
				if (direction == 0){on_rx -= 1;}
				else if(direction == 1){on_ry -= 1;}
				else if(direction == 2){on_rx += 1;}
				else {on_ry += 1;}
			}
			
			else if(ret == 1 || ret == 3)
			{
				grid[on_ry][on_rx] = ((255*point)/points);

				place = true;
			}
			
			else if(ret == 2){direction = rand() % 4;}
			
			else if (ret == 4){printf("%s", "WARN: Failed ret");}
			
			else{place =true;}
		}

	}

	
	
	

	FILE *file;
	file = fopen("./dla_out.bmp", "wb");

	if (file == NULL){printf("failed to open file.");}

	int needed_padding = 4-(width*3)%4;

	write_header(file, width, height);			// generate the header of the bitmap and write it to the file.
	


	unsigned char zer = 0;					// zero.
								// ...
								// i need to figure out something better...

	


	for( int y = 0; y < height; y++)			// itterate through list, printing it
	{							// and putting it into the bitmap.
		for( int x = 0; x < width; x++)				
		{						// currently it flips it, 
								// as the array is top-right 
								// to bottom-left, while the 
								// bitmap is the inverse, so
								// the terminal output will be
								// flipped from the bitmap.


			if (grid[y][x] == 0)			// if cell has nothing
			{					// print fully dark character
								// and add to bmp
				fwrite(&zer, 1, 1, file);
				fwrite(&zer, 1, 1, file);
				fwrite(&zer, 1, 1, file);
				
				printf(
					"\033[38;2;%i;%i;%i;48;2;%i;%i;%im \033[48;2;0;0;0m", 
					0,0,0, 
					0,0,0
					);


			}

			else					// if cell has something
			{					// print that something
				int pt = 255-grid[y][x];	// and add it to bmp


				r = (3*pt);			// functions to change
				g = (3*pt-255*2);		// colour based on when
        			b = (3*pt-255);			// it was placed
				


				if (r <= 0){r = 0;}
				else if (r >= 255){r = 255;}

				if (g <= 0){g = 0;}
				else if (g >= 255){g = 255;}

				if (b <= 0){b = 0;}
				else if (b >= 255){b = 254;}

				unsigned char r_char = (unsigned char)(r);
				unsigned char g_char = (unsigned char)(g);
				unsigned char b_char = (unsigned char)(b);
				
				printf(
					"\033[38;2;%i;%i;%i;48;2;%i;%i;%im#\033[48;2;0;0;0m", 
					r, g, b, 
					r, g, b
					);

				fwrite(&b_char, 1, 1, file);
				fwrite(&g_char, 1, 1, file);
				fwrite(&r_char, 1, 1, file);



			}
			
		
		}


		for(int padded = 0; padded < needed_padding; padded++)
		{
			fwrite(&zer, 1, 1, file);			// add padding to bmp
		}

		printf("%s", "\n");
	}
	printf("%s", "Finished!");
	fclose(file);
	return 0;
}

