#include "FieldProperty.H"


void FieldProperty::loopNSWO(Lattice& theLattice,LatticeIterator& LatIter,Field<FluidState>& Fluid,Field<NeighbourState>& Neighbours)
{   //  Allocation of the neighbour states to the FS-cells

    for(LatIter.begin(); !LatIter.end(); LatIter++) 
    {
	if(Fluid(*LatIter).state == FS)            //fluid bondary cells
	{   
	    NeighbourIterator NeighIter(*LatIter);  
            //(*LatIter) = inPoint für NeighbourIterator
  	    Point neighP = *NeighIter;


   	    if(NeighIter.direction() == 0 && Fluid(*NeighIter).state == EN)      
	    {
		NeighIter++;
		Neighbours(*LatIter).stateNeigh = N;                //North

		if(NeighIter.direction() == 1 && Fluid(*NeighIter).state == EN)  
		{
		    NeighIter++;
		    Neighbours(*LatIter).stateNeigh = NO;           //NO

		    if(NeighIter.direction() == 2 && Fluid(*NeighIter).state == EN) 
		    {
			NeighIter++;
			Neighbours(*LatIter).stateNeigh = NSO;      //NSO

			if(NeighIter.direction() == 3 && Fluid(*NeighIter).state == EN)
			{
			    Neighbours(*LatIter).stateNeigh = NSWO; //NSWO
			}
		    }
		    else
		    {
			NeighIter++;
			if(NeighIter.direction() == 3 && Fluid(*NeighIter).state == EN)
			{
			    Neighbours(*LatIter).stateNeigh = NWO;  //NWO
			}
		    }
		}
		else
		{
		    NeighIter++;

		    if(NeighIter.direction() == 2 && Fluid(*NeighIter).state == EN)
		    {
			NeighIter++;
			Neighbours(*LatIter).stateNeigh = NS;       //NS

			if(NeighIter.direction() == 3 && Fluid(*NeighIter).state == EN)
			{
			    Neighbours(*LatIter).stateNeigh = NSW;  //NSW
			}
		    }

		    else
		    {
			NeighIter++;

			if(NeighIter.direction() == 3 && Fluid(*NeighIter).state == EN)
			{
			    Neighbours(*LatIter).stateNeigh = NW;   //NW
			}
		    }
		}
	    }
	    else    //North != EN
	    {
		NeighIter++;

		if(NeighIter.direction() == 1 && Fluid(*NeighIter).state == EN)
		{
		    NeighIter++;
		    Neighbours(*LatIter).stateNeigh = O;            //East

		    if(NeighIter.direction() == 2 && Fluid(*NeighIter).state == EN)
		    {
			NeighIter++;
			Neighbours(*LatIter).stateNeigh = SO;       //SO
			if(NeighIter.direction() == 3 && Fluid(*NeighIter).state == EN)
			{
			    Neighbours(*LatIter).stateNeigh = SWO;  //SWO   
			}
		    }
		    else
		    {
			NeighIter++;
			if(NeighIter.direction() == 3 && Fluid(*NeighIter).state == EN)
			{
			    Neighbours(*LatIter).stateNeigh = WO; //WO 
			}
		    }
		}
		else
		{
		    NeighIter++;

		    if(NeighIter.direction() == 2 && Fluid(*NeighIter).state == EN)
		    {
			NeighIter++;
			Neighbours(*LatIter).stateNeigh = S;        //South

			if(NeighIter.direction() == 3 && Fluid(*NeighIter).state == EN)
			{
			    Neighbours(*LatIter).stateNeigh = SW; //SW
			}
		    }
			
		    else
		    {
			NeighIter++;

			if(NeighIter.direction() == 3 && Fluid(*NeighIter).state == EN)
			{
			    Neighbours(*LatIter).stateNeigh = W;    //West
			}
		    }
		}
	    }
	}

	else  //for all other cells which are not FS
	{
	    Neighbours(*LatIter).stateNeigh = noNSWO;             //noNSWO = 0
	} 
    }
}


void FieldProperty::loopENtoFS(Lattice& theLattice,LatticeIterator& LatIter,Field<real>& depositHeight,Field<FluidState>& Fluid)
{   //  Allocation of the fluid states E, EN, FS, F to the cells

    for(LatIter.begin(); !LatIter.end(); LatIter++) 
    {
    	if(depositHeight(*LatIter) != 0)	
    	    Fluid(*LatIter) = F;    //F=3
    	else
    	    Fluid(*LatIter) = E;    //E=0
    }

    for(LatIter.begin(); !LatIter.end(); LatIter++) 
    {
    	if(Fluid(*LatIter).state == F)
    	{
    	    NeighbourIterator NeighIter(*LatIter);  
                //(*LatIter) = inPoint für NeighbourIterator
      	    Point neighP = *NeighIter;          
    
    	    for(NeighIter.begin(); !NeighIter.end(); NeighIter++)
    	    {
        		if(Fluid(*NeighIter).state == E)    
        		{
        		    Fluid(*LatIter) = FS;   //FS=2
        		    break; //to leave the inner for-tool
        		}
    	    }
    	}
    }

    for(LatIter.begin(); !LatIter.end(); LatIter++) 
    {
    	if(Fluid(*LatIter).state == FS)
    	{
    	    NeighbourIterator NeighIter(*LatIter);  
                //(*LatIter) = inPoint für NeighbourIterator
      	    Point neighP = *NeighIter;
    
    	    for(NeighIter.begin(); !NeighIter.end(); NeighIter++)
    	    {
        		if(Fluid(*NeighIter).state == E)   
        		{
        		    Fluid(*NeighIter) = EN;   //EN=1
        		}
    	    }
    	}
    }

    
}


void FieldProperty::loopOnlyFS(Lattice& theLattice,LatticeIterator& LatIter,Field<FluidState>& Fluid)
{
    for(LatIter.begin(); !LatIter.end(); LatIter++) 
    {
    	if(Fluid(*LatIter).state == FS)
    	{
    	    NeighbourIterator NeighIter(*LatIter);  
    	    //(*LatIter) = inPoint für NeighbourIterator
    	    Point neighP = *NeighIter;
    
    	    for(NeighIter.begin(); !NeighIter.end(); NeighIter++)
    	    {
        		if(Fluid(*NeighIter).state == E)   
        		{
        		    Fluid(*NeighIter) = EN;   //EN=1
        		}
    	    }
    	}
    }
}

real FieldProperty::numberNSWO(Lattice& theLattice,LatticeIterator& LatIter, Field<NeighbourState>& Neighbours, Point inPoint)
{
    NeighbourIterator NeighIter(inPoint);  

    Point neighP = *NeighIter;

    int numberNSWOneighbours = 0;

    for(NeighIter.begin(); !NeighIter.end(); NeighIter++)
    {
    	if(Neighbours(*NeighIter).stateNeigh != 0)   
    	{
            numberNSWOneighbours ++;   
    	}
    }
    
    return numberNSWOneighbours;
}
    
