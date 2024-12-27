#include "derivative.h" 
#include "TFC\TFC.h"

int main(void)
{

	// Initialisation de la carte
	TFC_Init();
	
	for(;;)
	{	   
		    //TFC_Task must be called in your main loop.  This keeps certain processing happy (I.E. Serial port queue check)
			TFC_Task();

			
	}
	
	return 0;
}
