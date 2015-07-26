# SmartFile
Extension of class std::fstream
The main feature of this class is that every file knows in which mode it was opened. 
It gives You opportunity to use one shared file in many objects at the same time. 
Every object can get the current file mode and re-open it if it's needed.
