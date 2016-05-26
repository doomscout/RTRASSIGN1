Real-Time Rendering & 3D Games Programming Assignment 1

student group:
Ke Yi Ren s3405167
William Field s3434180

Assignment1 file: main cpp, Point3f.cpp, SineWave3D.cpp, Readme, RTR Ass1 Report.pdf, Assignment 1 Resualts.xlsx(Excel)
Note: a lot of cpp file is not used and they all in unused Folder

Makefile folder location - Assignment1/Source/
Run/execution program 's folder  -Assignment1/Bin/

commandline example:
go to Assignment1/Source folder
then
type "make"
open 2nd terminal
go to Assignment1/Bin folder
type ./Assignment1


Assignment 1 hotkey for program
w - change waveLength of Sinewave3D
s - change display or multi display viewport
d - change drawMethod between Immediation and Vertex Buffer Object mode
c - enable console's printout to report benchmark performance 
l - enable/disable lighting
f - enable/disable 30 fps mode
o - enable/disable OSD 
k - increase lighting number by one (only Max is 9 numbers)
j - decrease lighting number by one (only minimum is 1 numbers)
n - enable/disable shows Draw Vector of normal
m - change  polygon mode of display between Fill and Wireframe
+ - increase tessellation number of SineWave3D
- - decrease tessellation number of SineWave3D
escape - exit program


							Known Issue:

VBO lighting fps issue:
disable glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) for Sutherland Laboratory otherwise it will decrease to 1 fps from 300 fps....
however my home desktop computer with AMD/Radeon GPU shows no issue if enable glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE)

VBO animation fps issue:
in theory/paper, VBO should be better than Immediation mode for animation and lighting. However seems VBO can't better than Immediate mode with animation only(No Light)

							Fixed ISSUE:
Issue #1: Crash when reach to 1024 tessellation
	Reason: mistakely setting up for loop in Sinewave3D::initSineWave() Function
	for(i++)
	{
		for{j++}//
		
		for{i < indices, i++} ///WWOOOOP CRASH!
	}

Is it fixed : yes
	for(i++)
	{
		for{j++}	
	}
	for{indices}



