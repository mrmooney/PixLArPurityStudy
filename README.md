# PixLArPurityStudy
Tool for Measurement of Electron Lifetime at PixLAr

  Instructions:
  
    1. log into a lariatgpvm machine (do not set up any local products, e.g. lariatsoft)
    
    2. cd to working directory with PixLArPurityStudy package (e.g. /lariat/app/users/YOURUSERNAME/work/PixLarPurityStudy/)
    
    3. to measure electron lifetime for run XYZ, do:  source runAll.sh XYZ
    
    4. if you want to measure electron lifetime for run XYZ but you've already run once, i.e. produced the files with RawDigits ("files" directory in your working directory), do:  source runAll.sh XYZ rerun
    
  
  Notes:
  
    1. this is a primitive measurement mainly for the purposes of PixLAr operations
    
    2. this should only be used with runs containing mostly cosmic triggers (has not been tested with beam runs)
    
    3. if you have questions, feel free to email the author (donations/pity welcome):  mrmooney@colostate.edu
