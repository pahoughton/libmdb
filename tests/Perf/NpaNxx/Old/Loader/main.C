//
// File:        main.C
// Desc:        
//
//  
//  
// Locationor:      Igor Chudov - (ichudov@shoe.wiltel.com)
// Created:     06/26/95 09:13 
//
// Revision History:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:47  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:23  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/19 12:52:43  houghton
// Initial Version (work in progess).
//
// Revision 1.7  1996/10/09  22:33:03  mpritcha
// Fixed misspelling.
//
// Revision 1.6  1996/09/30  16:53:52  mpritcha
// Added StatsAndErrMsgs class.
//
// Revision 1.5  1996/08/20  00:41:36  ichudov
// AppPAram works correctly.
//
// Revision 1.4  1996/07/29  14:23:20  rjankir
// Added dummy entry for location 999999. Actually this
// dummy entry is to to support the dummy entry
// put in the TrunkLoader for Calls with not final and term
// trunk.
//
// Revision 1.3  1995/09/07  21:10:07  ichudov
// Added return value to main().
//
// Revision 1.2  1995/08/29  22:18:04  ichudov
// Caribbean loading.
//
// Revision 1.1  1995/08/25  22:13:12  ichudov
// New module added.
//
//
static const char * RcsId =
"$Id$";

#include <StatsAndErrMsgs.hh>
#include <NpaNxxDeltaSybase.hh>
#include <CARNpaNxxDeltaSybase.hh>
#include <LocationTable.hh>
#include <AppParam.hh>
#include <DeltaNpaNxx.hh>
#include <Delta.hh>

LocationTable * NpaNxxTable;
AppParam   * App;

StatsAndErrMsgs sem;    //   Standard record count
StatsAndErrMsgs ncsem; //  Ncarr record count

char recBuf[256];                                      //  Buffer to hold any record to be written
char fieldName[50];                                   // Field causing the error
char outFile[50];                                     //  Name of file to write the record to for
                                                      //  later evaluation
char procName[] = "NPANXXDELTA";
char nprocName[] = "NPANXX - NCARR-DELTA";
char msgs[256];
int  writeRec = 0;
int type = 0;
int errCode = 0;  
int countRec= 0;

main(int argc, char * argv[]) 
{

  App = new AppParam(&argc, argv);
 
  if( App->help() || !App->good() )
    {
	cerr << *App << endl;
        errCode = 3;   //  PROCESSHALT;
        strcpy( fieldName, "Parameters");
        sem.PrintErr( errCode, fieldName, msgs, procName);
	exit(1);
    }
  
  NpaNxxDeltaSybase   input( App->sybServer(),
			   App->sybDatabase(),
			   App->sybUser(),
			   App->sybPassword() );

  CARNpaNxxDeltaSybase   inputCAR( App->sybServer(),
			   App->sybDatabase(),
			   App->sybUser(),
			   App->sybPassword() );

  if( !input.good() )
    {
	strcpy( msgs,  input.error() );
        errCode = 3;   //  PROCESSHALT;
        strcpy( fieldName, "Sybase Parameters" );
        sem.PrintErr( errCode, fieldName, msgs, procName);
	exit(1);
    }
  else
    {
      // cout << "Sybase connection Succeeded ! " << endl;
    }


  if(App->isLoad())
    {
         NpaNxxTable = new LocationTable(App->NpaNxxKeyFile(),
                                       App->NpaNxxDataFile(),
                                       App->mapChunkSize()); 
    }
  else
    {
	 NpaNxxTable = new LocationTable(App->NpaNxxKeyFile(),
				     App->NpaNxxDataFile(),
				   (ios::open_mode) (ios::in | ios::out));
    }

  if( !NpaNxxTable->good() )
    {
      errCode = 3;   //  PROCESSHALT;
      strcpy( fieldName, "NpaNxxTable");
     strcpy(msgs,  NpaNxxTable->error() );
     sem.PrintErr( errCode, fieldName, msgs, procName);          
      exit(1);
    }

  DeltaNpaNxx	delta( NpaNxxTable, App->effDateOverride().getTimeT() );

  for( ; ;  )
    {
      //      if( App->progress() && ! (input.getRecCount() % App->progress() ) )
      //        {
      //  cerr << input.getRecCount() << "\r";
      //  cerr.flush();
      //        }

       if ( ! input.next() )
	{
          if ( input.good() )  
            {
              if( input.done())
		{
	          break;
		}
            }
	  else 
            {
              errCode = 3;   //  PROCESSHALT;
              strcpy( fieldName, "Input File");
              strcpy(msgs,  input.error() );
              sem.PrintErr( errCode, fieldName, msgs, procName);          
	      exit(1);
            }
	}
      else
	{
          LocationRec rec( input.getNpaNxx(),   input.getName(), input.getGeo(),
                       input.getRegionAbbr(),  input.getLata(),
		       input.getVert(), input.getHorz(),
		       input.getTZOffset(), input.isDSTHonored());


          DateTime effDate( input.getEffDate().getTimeT() );

          if( ! delta.delta( input.getRecType(), rec, effDate ) )
           {
             if( ! delta.good() )
               {
              errCode = 1;
	      strcpy(msgs, delta.error());
	      strcpy(fieldName, "Record");
	      if (countRec == YES) {
	      sem.GatherStats(errCode, type, procName );
	      }
	      sem.PrintErr(errCode, fieldName, msgs, procName);
	 }
             else
               {
        	      errCode = 1;
	      strcpy(msgs, delta.getWhy());
	      strcpy(fieldName, "record");
	      if (countRec == YES) {
	           sem.GatherStats(errCode, type, procName );
	      }
	      sem.PrintErr(errCode, fieldName, msgs, procName);
               }
           }      
	}
    }


  //
  // This dummy record is to rate Calls which don't have a complete Dialled
  // Number and Final Trunk. These call were dropped as non billable before
  // IX Transport. With IX Transport, we can charge Access Cost on these
  // call, if the originated from a switched trunk. 
  //
  LocationRec dummyRec(999999,"UNKNOWN",LocationRec::makeGeo("INE" ), "XX");
  DateTime dummyDate("700101");
  strcpy (recBuf, "999999UNKNOWNINEXXX" );
  if( ! delta.delta( Delta::ADD, dummyRec, dummyDate ) )
   {
      if( ! delta.good() )
      {
       	   errCode = 1;
	   strcpy(msgs, delta.error());
	   strcpy(fieldName, "Record");
	   if (countRec == YES) {
	   ncsem.GatherStats(errCode, type, nprocName );
	   }
	   ncsem.PrintErr(errCode, fieldName, msgs, nprocName);
       }
      else
      {
          	errCode = 1;
	     strcpy( msgs, recBuf );
	     strcat( msgs,"effective date ");
	     strcat( msgs, dummyDate );
	     strcat(msgs, delta.getWhy());
	     strcpy(fieldName, "InputRec");
	     if (countRec == YES) {
	          ncsem.GatherStats(errCode, type, nprocName );
	     }
	     ncsem.PrintErr(errCode, fieldName, msgs, nprocName);
      }           
   }    
			
  for( ; ;  )
    {
      if( App->progress() && ! (inputCAR.getRecCount() % App->progress() ) )
        {
	  //          cerr << inputCAR.getRecCount() << "\r";
	  //          cerr.flush();
        }

      if ( ! inputCAR.next() )
	{
          if ( inputCAR.good() )  
            {
              if( inputCAR.done())
		{
	          break;
		}
            }
	  else 
            {
     	 errCode = 3;
	      strcpy(msgs, inputCAR.error());
	      strcpy(fieldName, "inputCAR");
	      if (countRec == YES) {
	      ncsem.GatherStats(errCode, type, nprocName );
	      }
	      ncsem.PrintErr(errCode, fieldName, msgs, nprocName);
	      exit(1);
            }
	}
      else
        {
           LocationRec rec( inputCAR.getNpaNxx(), inputCAR.getName(), 
		       inputCAR.getGeo(),
                       inputCAR.getRegionAbbr(),  inputCAR.getLata(),
	               inputCAR.getVert(), inputCAR.getHorz(),
		       inputCAR.getTZOffset(), inputCAR.isDSTHonored());

//      cout << "rec = " << rec << endl;

        DateTime effDate( inputCAR.getEffDate().getTimeT() );

        LocationRec key( inputCAR.getNpaNxx() );
        if( ( NpaNxxTable->find( key.getKey(), effDate ) ) )
  	  {
	    continue;
          }
        if( ! delta.delta( inputCAR.getRecType(), rec, effDate ) )
          {
            if( ! delta.good() )
              {
                 errCode = 1;
	   strcpy(msgs, delta.error());
	   strcpy(fieldName, "Delta Record");
	      if (countRec == YES) {
	      ncsem.GatherStats(errCode, type, nprocName );
	      }
	      ncsem.PrintErr(errCode, fieldName, msgs, nprocName);
              }
            else
              {
                errCode = 1;
	      strcpy(msgs, delta.getWhy());
	      strcpy(fieldName, "record");
	      if (countRec == YES) {
	           ncsem.GatherStats(errCode, type, nprocName );
	      }
	      ncsem.PrintErr(errCode, fieldName, msgs, nprocName);
              }
          }      
      }
      countRec = YES;
    }

  sem.PrintStats(procName);
  ncsem.PrintStats(nprocName);
  return( 0 );

}



//              This software is the sole property of
// 
//                 The Williams Companies, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1995 by The Williams Companies, Inc.
// 
//                      All Rights Reserved.  
// 
//
