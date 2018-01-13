# RRR
RRR means randomly replace recovery scheme, which is effective for the data reconstruction of any XOR-based erasure codes. In the current version, we mainly implement three algorithms for experiments: Zpacr,the hill-climbing algorithm; RRR, the simulated annealing algorithm; enumeration, the enumeration algorithm. This platform is based on the Jerasure-1.2 library, which contains a variety of erasure codes: Reed_Sol_Van, Cauchy_Orig, Cauchy_Good, Liberation, Blaum_Roth, Liber8tion.
Author: lyfdamon

------------------------------------------------------------------------------------------------------------------------------------------
## The file directory tree of the platform
<<<<<<< HEAD

RRR/
├── algorithm
│   ├── climb_search
│   │   ├── include
│   │   │   └── climb_search.h
│   │   ├── Makefile
│   │   └── src
│   │       └── climb_search.cpp
│   ├── enumeration_search
│   │   ├── include
│   │   │   └── enumeration_search.h
│   │   ├── Makefile
│   │   └── src
│   │       └── enumeration_search.cpp
│   ├── Makefile
│   └── sa_search
│       ├── include
│       │   └── sa_search.h
│       ├── Makefile
│       └── src
│           └── sa_search.cpp
├── Example
│   ├── climb_test.cpp
│   ├── enumeration_test.cpp
│   ├── Makefile
│   └── sa_test.cpp
├── jerasure
│   ├── include
│   │   ├── cauchy.h
│   │   ├── galois.h
│   │   ├── jerasure.h
│   │   ├── liberation.h
│   │   └── reed_sol.h
│   ├── Makefile
│   └── src
│       ├── cauchy.cpp
│       ├── galois.cpp
│       ├── jerasure.cpp
│       ├── liberation.cpp
│       └── reed_sol.cpp
├── Makefile
├── Makefile.build
└── README.md																							    	
=======
RRR                                                                                                                                     
|---- algorithm                                                                                                                         
|   |-- climb_search                                                                                                                    
|   |   |-- include                                                                                                                     
|   |   |   |-- climb_search.h                                                                                                          
|   |   |-- Makefile                                                                                                                    
|   |   |-- src                                                                                                                         
|   |       |-- climb_search.cpp                                                                                                        
|   |-- enumeration_search                                                                                                              
|   |   |-- include                                                                                                                     
|   |   |   |-- enumeration_search.h                                                                                                    
|   |   |-- Makefile                                                                                                                    
|   |   |-- src                                                                                                                         
|   |       |-- enumeration_search.cpp                                                                                                  
|   |-- Makefile                                                                                                                        
|   |-- sa_search                                                                                                                       
|       |-- include                                                                                                                     
|       |   |-- sa_search.h                                                                                                             
|       |-- Makefile                                                                                                                    
|       |-- src                                                                                                                         
|           |-- sa_search.cpp                                                                                                           
|-- Example                                                                                                                             
|   |-- climb_test.cpp                                                                                                                  
|   |-- enumeration_test.cpp                                                                                                            
|   |-- Makefile                                                                                                                        
|   |-- sa_test.cpp                                                                                                                     
|-- jerasure                                                                                                                            
|   |-- include                                                                                                                         
|   |   |-- cauchy.h                                                                                                                    
|   |   |-- galois.h                                                                                                                    
|   |   |-- jerasure.h                                                                                                                  
|   |   |-- liberation.h                                                                                                                
|   |   |-- reed_sol.h                                                                                                                  
|   |-- Makefile
|   |-- src
|       |-- cauchy.cpp
|       |-- galois.cpp
|       |-- jerasure.cpp
|       |-- liberation.cpp
|       |-- reed_sol.cpp
|-- Makefile
|-- Makefile.build
|-- README.md
>>>>>>> b641877f58b123b674b73e78ac2fa13ec2de56eb
