#include "Splat.H"

real Splat::maxAbsField(Field<real> &field)
{
    LatticeIterator LatIter(theLattice); //LatticeIterator erzeugen
    real value = 0;
    
    for(LatIter.begin(); !LatIter.end(); LatIter++)  
    {
        if (value < abs(field(*LatIter)) && abs(field(*LatIter))!= abs(UNREAL_VELOCITY))
            value = abs(field(*LatIter));
    }
    return value;
}

void Splat::Reset()
{
    //Partikel als geschmolzen setzen
    molten = true;
    
    LatticeIterator LatIter(theLattice); //LatticeIterator erzeugen
    impactPoint = new Point ((int)round(theLattice.Lx()/2.),(int)round(theLattice.Ly()/2.),theLattice); //Auftreffpunkt
    
    Point latP = * LatIter; //Nullpunkt
    
    //Teilchen mit festem Radius
    droplet = new Particle(theProblem->partMeanRadius, theProblem->partMeanVel, theProblem->partViscosity,
			   theProblem->partDensity, theProblem->impactAngle, theProblem->directionAngle, true, *impactPoint,
			   theProblem->slowDownFactor, theProblem->slowDownNorm);

    //Teilchen mit random Radius
    //double randomPartMeanRadius = (double)Rand.uniform(theProblem->partMeanRadiusMIN,theProblem->partMeanRadiusMAX);
    //droplet = new Particle(randomPartMeanRadius, theProblem->partMeanVel, theProblem->partViscosity, theProblem->partDensity,
    //theProblem->impactAngle, theProblem->directionAngle,true, *impactPoint,
    //theProblem->slowDownFactor, theProblem->slowDownNorm);
			 
    // kleiner Startkreis mit Radius = markerRadius
    for(LatIter.begin(); !LatIter.end(); LatIter++)  
    {
   	double DX = theLattice.dx()*(impactPoint->x - (*LatIter).x);
    	double DY = theLattice.dy()*(impactPoint->y - (*LatIter).y);
    	double distance = sqrt(pow(DX,2) + pow(DY,2));

    	if(distance < theProblem->markerRadius)
    	{
            depositHeight(*LatIter) = droplet->heightMadejski();
        }
        else
    	{
            depositHeight(*LatIter) = 0; 
        }
    }
    
    //Zuweisung Fluidstate
    FieldProperty::loopENtoFS(theLattice, LatIter, depositHeight, Fluid);

   //Zuweisung Neighbourstate an alle FS
    FieldProperty::loopNSWO(theLattice, LatIter, Fluid, Neighbours);
       
    //Marker-Partikel setzen
    marker = new Marker(theLattice,Fluid, theProblem->markerRadius/theProblem->dy, *impactPoint, theProblem->impactAngle, theProblem->directionAngle);
    
    std::cout<<std::endl;
    std::cout<<"Startkonfiguration:"<<std::endl;
    OutputParameters();
}

void Splat::OutputParameters()
{
    WRITE(theLattice.Lx());
    WRITE(theLattice.Ly());
    WRITE(theLattice.dx());
    WRITE(theLattice.dy());
    WRITE(theProblem->numParticles);
    WRITE(theProblem->partMeanRadius);
    WRITE(theProblem->partMeanVel);
    WRITE(theProblem->partViscosity);
    WRITE(theProblem->partDensity);
    WRITE(droplet->impactAngle);
    WRITE(droplet->directionAngle);
    WRITE(droplet->Reynolds());
    WRITE(droplet->volume());
    WRITE(droplet->heightMadejski());
}

Splat::Splat(Problem &prob, Field<real> &currentHeight)
{
    theProblem = &prob;
    //Creating a lattice with theProblem (pushDefault or Testin)
    theLattice = Lattice(theProblem->numLxPoints, theProblem->numLyPoints, theProblem->dx, theProblem->dy);
    
    //Felder definieren
    height = currentHeight;        //aktuelle Substrathöhe (z.B. Hsub + H von zig Splats)  
    depositHeight = Field<real> (theLattice); //aktuelle Splathöhe (Dicke des abzulegenden Splats)
    Fluid = Field<FluidState> (theLattice);  //Feld fuer Fluidstate (E, EN, FS, F)
    Neighbours = Field<NeighbourState> (theLattice); //Feld fuer Neighbourstate (N, S, O, W, NW, NO ...)
    //F, FS etc. nur noch fuer Darstellung in all*.eps interessant
    
    Reset();
}

Splat::~Splat()
{
    //Speicher freigeben
    if (impactPoint != NULL) delete (impactPoint);
    if (droplet != NULL) delete (droplet);     
    if (marker != NULL) delete (marker);  
}

double Splat::depositedVolume(void)
{   
    //Aufsummieren des abgelagerten Volumens 
    real SumDepositVol = 0;    
    LatticeIterator LatIter(theLattice);     

    for(LatIter.begin(); !LatIter.end(); LatIter++)
    SumDepositVol += (theLattice.dx()*theLattice.dy()*depositHeight(*LatIter));

    return SumDepositVol;
}

double Splat::CutFluid(void)
{
    double redundantVolume = 0; //ueberschuessiges Volumen, noch zu verteilen
    
    //jedes Feldelement durchgehen
    LatticeIterator LatIter(theLattice); //LatticeIterator erzeugen    

    for(LatIter.begin(); !LatIter.end(); LatIter++)
    {
        //wenn die Feldhoehe die errechnete Madejski-Hoehe ueberschreitet
        if(depositHeight((*LatIter)) > droplet->heightMadejski())
        {
            //ueberschuessiges Volumen berechnen
            redundantVolume += (theLattice.dx()*theLattice.dy()*(depositHeight(*LatIter) - droplet->heightMadejski()));
            depositHeight(*LatIter) = droplet->heightMadejski(); // auf Madejski-Hoehe zurueckschneiden
        }
    }
    
    return redundantVolume;
}

Field<real> Splat::Melt(void)
{
    LatticeIterator LatIter(theLattice);

    std::cout<<std::endl;
    std::cout<<"Starting the spreading of a molten particle."<<std::endl;

    if(molten)
    {
    	real splatVolStart = M_PI * pow(theProblem->markerRadius,2) * droplet->heightMadejski();
        WRITE(splatVolStart);  //Volumen des Startkreises
    
    	real VolumeToDeposit = droplet->volume() - splatVolStart;
        WRITE(VolumeToDeposit); //noch zu verteilendes Volumen
    	std::cout<<std::endl;
        
        circulationCount = 0;
        	
	Output("all"); //Ausgabe der Startkonfiguration
	    
        //Variable ueberschuessiges Volumen = noch zu verteilendes Volumen
        double redundantVolume = VolumeToDeposit; 

        do //Ausbreiten, solange VolumeToDeposit und redundantVolume beide positiv
        {
            //Markervolumen setzen
    	    marker->SetVolume(redundantVolume, droplet->heightMadejski());
            
           do //Ausbreitungsalgorithmus
           {
               circulationCount ++;
                
               marker->CalcNewPosition(theLattice, Fluid, depositHeight, height); //Marker bewegen & Material ablagern
               marker->SlowDownMarker(droplet->slowDownFactor, droplet->slowDownNorm); 
               //Markerbewegung verlangsamen (findet keine Anwendnugn, Faktor = 1, Norm = 0
                    
               WRITE(circulationCount);
                	
               Output("all"); //Ausgabe aller Zwischenschritte
               std::cout<<std::endl;
                	
            } while (!marker->VolumeZero());
             
            //Höhenfeld beschneiden u. überschüssiges Volumen berechnen
            redundantVolume = CutFluid(); 
            VolumeToDeposit = droplet->volume() - depositedVolume();

	    cout << "abgeschnittenes Volumen (redundantVolume): "<< redundantVolume << endl;
            cout << "bisher abgelagertes Volumen (depositedVolume): "<< depositedVolume() << endl;
            cout << "noch abzulagerndes Volumen (VolumeToDeposit): "<< VolumeToDeposit  << endl;
	    //cout << "redundantVolume + abgelagertes Volumen: " << redundantVolume + depositedVolume() << endl;
	    std::cout<<std::endl;
                
        } while (VolumeToDeposit >= 0 && redundantVolume > 0);
        
    } //END if(droplet->molten)

    else
    {
	std::cout<<"Particle is not molten!"<<std::endl;
    }
    
    WRITE(depositedVolume());
    WRITE(marker->GetDeadVolume());
    WRITE(depositedVolume()+marker->GetDeadVolume());
    WRITE(droplet->volume());
    Output("all"); //Ausgabe
    depositHeight.writetofile(); 
    return depositHeight;
}

void Splat::Output(string filename)
{
    if (theProblem->epsOutput == 1)
    {
        LatticeIterator LatIter(theLattice); 

        //Schreiben der FS-Zellen
        string f1 = filename + itoa(1) +  ".dat";
        ofstream f_1 (f1.c_str(), ios::out|ios::binary);

        for(LatIter.begin(); !LatIter.end(); LatIter++)  
        {
            if (Fluid(*LatIter).state == FS)
                f_1<<LatIter.getX()+0.5<<" "<<-(LatIter.getY()+0.5)<<" "<<Fluid(*LatIter)<<std::endl;
        } 
        f_1<<std::endl<<std::endl;

        //Schreiben der F-Zellen
        for(LatIter.begin(); !LatIter.end(); LatIter++)  //Field depositVol aufsummieren
        {
             if (Fluid(*LatIter).state == F)
                f_1<<LatIter.getX()+0.5<<" "<<-(LatIter.getY()+0.5)<<Fluid(*LatIter)<<std::endl;
        }
        f_1.close();
        
        marker->Output();
        
        //Gnuplot ausfuehren
	system("rm -f gnuplot_all.gpl"); //alte Datei gnuplot_all.gpl wird gelöscht
	ofstream gnu("gnuplot_all.gpl"); //neue Datei gnuplot_all.gpl wird erzuegt

        //schreiben der Datei gnuplot_all.gpl; 
	//x- und yrange in Abh. der lokalen Gitterparameter setzen !!
	gnu << "set term post portrait color 'Times-Roman' 14" << std::endl; 
	gnu << "set output 'all.eps'" << std::endl; 
	gnu << "set size square" << std::endl; 
	gnu << "set xrange [0:" << theProblem->numLxPoints <<  "]" << std::endl; 
	gnu << "set yrange [" << -theProblem->numLxPoints  << ":0]" << std::endl; 
	gnu << "set style line 1 lt 7 lw 1" << std::endl; 
	gnu << "set style line 2 lt 0 lw 1" << std::endl; 
	gnu << "set xtics 20 " << std::endl; 
	gnu << "set ytics 20" << std::endl; 
	gnu << "set mxtics 20" << std::endl; 
	gnu << "set mytics 20" << std::endl; 
	gnu << " " << std::endl; 
	gnu << "set grid  xtics ytics mxtics mytics ls 1, ls 2" << std::endl; 
	gnu << "plot 'all1.dat' index 0 using 1:2 notitle with points lt 5 pointtype 5 pointsize 0.63," ; 
	gnu << "     'all1.dat' index 1 using 1:2 notitle with points lt 3 pointtype 5 pointsize 0.63, "; 
	gnu << "     'velocity.dat' using 1:2:3:4 notitle with vectors lt 1 lw 0.2,"; 
	gnu << "     'marker.dat' using 1:(-$2) notitle with points lt 15 pointtype 7 pointsize 0.1" << std::endl; 

        system("gnuplot gnuplot_all.gpl");
	string extraString="";
	if (circulationCount<10) extraString="00";
	else if (circulationCount<100) extraString="0";
        string file_new = "all" + extraString + itoa(circulationCount)+".eps";
        string file_newgif = "all" + extraString + itoa(circulationCount)+".gif";
	system("convert all.eps all.gif");  //Konvertierung von eps zu gif fuer ~/gifsicle (Animation erstellen)
       renamer::RenameFile("all.eps", file_new);  
       renamer::RenameFile("all.gif", file_newgif);  
    }
}
 
 




