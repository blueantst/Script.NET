////////////////////////////////////////////////////////////////////////////
//	File:		telnet.h
//	Version:	1.0.0.0
//	Created:	2001-10-05
//
//	Author:		blueant
//	E-mail:		script.net@gmail.com
//
//	Comm define of telnet protocal.
////////////////////////////////////////////////////////////////////////////
#ifndef __TELNET_H_
#define __TELNET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Telnet Command Code
const	unsigned char IAC		= 255;	// Interpret As Command Escape
const	unsigned char DO		= 253;	// Request To Do Option
const	unsigned char DONT		= 254;	// Request NOT To Do Option
const	unsigned char WILL		= 251;	// Desire/Confirm Will Do Option
const	unsigned char WONT		= 252;	// Refusal To Do Option
const	unsigned char SB		= 250;	// Start Subnegotiation
const	unsigned char SE		= 240;	// End Of Subnegotiation
const	unsigned char IS		= '0';	// TERMINAL_TYPE option "IS" command
const	unsigned char SEND		= '1';	// TERMINAL_TYPE option "SEND" command
const	unsigned char INFO		= '2';	// 
const	unsigned char VAR		= '0';	// 
const	unsigned char VALUE		= '1';	// 
const	unsigned char ESC		= '2';	// 
const	unsigned char USERVAR	= '3';	// 



/*------------------------------------------------------------------------*/
/* TELNET Command Codes: 												  */
/*------------------------------------------------------------------------*/
#define	TCSB        (unsigned char)250  /* Start Subnegotiation	          */
#define	TCSE		(unsigned char)240	/* End Of Subnegotiation          */
#define	TCNOP		(unsigned char)241	/* No Operation			          */
#define	TCDM		(unsigned char)242	/* Data Mark (for Sync)	          */
#define	TCBRK		(unsigned char)243	/* NVT Character BRK	          */
#define	TCIP		(unsigned char)244	/* Interrupt Process	          */
#define	TCAO		(unsigned char)245	/* Abort Output			          */
#define	TCAYT		(unsigned char)246	/* "Are You There?" Function      */ 
#define	TCEC		(unsigned char)247	/* Erase Character		          */
#define	TCEL		(unsigned char)248	/* Erase Line			          */
#define	TCGA		(unsigned char)249	/* "Go Ahead" Function		      */
#define	TCWILL		(unsigned char)251	/* Desire/Confirm Will Do Option  */
#define	TCWONT		(unsigned char)252	/* Refusal To Do Option		      */
#define	TCDO		(unsigned char)253	/* Request To Do Option		      */
#define	TCDONT		(unsigned char)254	/* Request NOT To Do Option	      */
#define	TCIAC		(unsigned char)255	/* Interpret As Command Escape	  */

/*------------------------------------------------------------------------*/
/* Telnet Option Codes: 											  */
/*------------------------------------------------------------------------*/
#define	TOTXBINARY	(unsigned char)  0	/* TRANSMIT-BINARY option	      */
#define	TOECHO		(unsigned char)  1	/* ECHO Option			          */
#define	TONOGA		(unsigned char)  3	/* Suppress Go-Ahead Option	      */
#define	TOTERMTYPE	(unsigned char) 24	/* Terminal-Type Option		      */

/*------------------------------------------------------------------------*/
/* Option Subnegotiation Constants: 									  */
/*------------------------------------------------------------------------*/
#define	TT_IS		0	        /* TERMINAL_TYPE option "IS" command	  */
#define	TT_SEND		1	        /* TERMINAL_TYPE option "SEND" command	  */

/*------------------------------------------------------------------------*/
/* Telnet Socket-Input FSM States: 										  */
/*------------------------------------------------------------------------*/
#define	TSDATA		 0	        /* normal data processing		          */
#define	TSIAC		 1	        /* have seen IAC			              */
#define	TSWOPT		 2	        /* have seen IAC-{WILL/WONT}		      */
#define	TSDOPT		 3	        /* have seen IAC-{DO/DONT}		          */
#define	TSSUBNEG	 4	        /* have seen IAC-SB			              */
#define	TSSUBIAC	 5	        /* have seen IAC-SB-...-IAC		          */

#define	NTSTATES	 6	        /* # of TS* states			              */

/*------------------------------------------------------------------------*/
/* Telnet Option Subnegotiation FSM States:                               */
/*------------------------------------------------------------------------*/
#define SS_START	0	        /* initial state			              */
#define	SS_TERMTYPE	1	        /* TERMINAL_TYPE option subnegotiation	  */
#define SS_IS       2
#define	SS_END		3	        /* state after all legal input		      */

#define	NSSTATES	4	        /* # of SS_* states			              */

#define	FSINVALID	0xff	    /* an invalid state number		          */

#define	NCHRS		256		    /* number of valid characters	          */
#define	TCANY		(NCHRS+1)	/* match any character		              */

//struct Fsm_Trans_S 
//{
//	unsigned char Ft_State ;	/* current state		                  */
//	short	      Ft_Char ;		/* input character		                  */
//	unsigned char Ft_Next ;		/* next state			                  */
//								/* action to take		                  */
//	int           ( * Ft_Action ) ( LPTask_Data_S, unsigned char ) ;
//} ;

#define	TINVALID	0xff	    /* an invalid transition index		      */



#endif // !defined(__TELNET_H_)