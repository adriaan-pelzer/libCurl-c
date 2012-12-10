#ifndef _ADDR_H_
#define _ADDR_H_

/***
  Something to thing about:

  It would be very cool if a bit of linguistic intelligence can be worked in to automatically generate ERR logs and DBG logs.
  Something along the lines of, instead of supplying the error string, (ie "Cannot process tweets"), rather supply an "action"
  string (ie "process tweets"), and then generate a DBG messages before the statement (ie "Start: process tweets") and an ERR
  string on failure (ie "Error: Cannot process tweets")
  ***/

#define FFF(X, FUNC, TEST) if (TEST) { FUNC(X); X = NULL; }
#define FF(X, FUNC) FFF(X, FUNC, X)
#define F(X) FF(X, free)

#define EA_PROTO(ERRTYPE, EXTRA, LOP, FUNC, ERRVAL, ERRSTR, ...) if ((LOP = FUNC(__VA_ARGS__)) == ERRVAL) { syslog(ERRTYPE, ERRSTR); EXTRA; }
#define EASD(...) EA_PROTO(P_DBG,, __VA_ARGS__) 
#define EACD(...) EA_PROTO(P_DBG,continue, __VA_ARGS__) 
#define EAS(...) EA_PROTO(P_ERR,, __VA_ARGS__) 
#define EAC(...) EA_PROTO(P_ERR,continue, __VA_ARGS__) 
#define EAD(...) EA_PROTO(P_DBG,goto over, __VA_ARGS__) 
#define EA(...) EA_PROTO(P_ERR,goto over, __VA_ARGS__) 

#define E2A_PROTO(ERRTYPE, EXTRA, LOP, FUNC, ERRVAL, ERRSTR, ERRVAR, ...) if ((LOP = FUNC(__VA_ARGS__)) == ERRVAL) { syslog(ERRTYPE, ERRSTR, ERRVAR); EXTRA; }
#define E2ASD(...) E2A_PROTO(P_DBG,, __VA_ARGS__) 
#define E2ACD(...) E2A_PROTO(P_DBG,continue, __VA_ARGS__) 
#define E2AS(...) E2A_PROTO(P_ERR,, __VA_ARGS__) 
#define E2AC(...) E2A_PROTO(P_ERR,continue, __VA_ARGS__) 
#define E2AD(...) E2A_PROTO(P_DBG,goto over, __VA_ARGS__) 
#define E2A(...) E2A_PROTO(P_ERR,goto over, __VA_ARGS__) 

#define ENA_PROTO(ERRTYPE, EXTRA, LOP, FUNC, NERRVAL, ERRSTR, ...) if ((LOP = FUNC(__VA_ARGS__)) != NERRVAL) { syslog(ERRTYPE, ERRSTR); EXTRA; }
#define ENASD(...) ENA_PROTO(P_DBG,, __VA_ARGS__) 
#define ENACD(...) ENA_PROTO(P_DBG,continue, __VA_ARGS__) 
#define ENAS(...) ENA_PROTO(P_ERR,, __VA_ARGS__) 
#define ENAC(...) ENA_PROTO(P_ERR,continue, __VA_ARGS__) 
#define ENAD(...) ENA_PROTO(P_DBG,goto over, __VA_ARGS__) 
#define ENA(...) ENA_PROTO(P_ERR,goto over, __VA_ARGS__) 

#define E2NA_PROTO(ERRTYPE, EXTRA, LOP, FUNC, NERRVAL, ERRSTR, ERRVAR, ...) if ((LOP = FUNC(__VA_ARGS__)) != NERRVAL) { syslog(ERRTYPE, ERRSTR, ERRVAR); EXTRA; }
#define E2NASD(...) E2NA_PROTO(P_DBG,, __VA_ARGS__) 
#define E2NACD(...) E2NA_PROTO(P_DBG,continue, __VA_ARGS__) 
#define E2NAS(...) E2NA_PROTO(P_ERR,, __VA_ARGS__) 
#define E2NAC(...) E2NA_PROTO(P_ERR,continue, __VA_ARGS__) 
#define E2NAD(...) E2NA_PROTO(P_DBG,goto over, __VA_ARGS__) 
#define E2NA(...) E2NA_PROTO(P_ERR,goto over, __VA_ARGS__) 

#define ET_PROTO(ERRTYPE, EXTRA, FUNC, ERRVAL, ERRSTR, ...) if ((FUNC(__VA_ARGS__)) == ERRVAL) { syslog(ERRTYPE, ERRSTR); EXTRA; }
#define ETSD(...) ET_PROTO(P_DBG,, __VA_ARGS__) 
#define ETCD(...) ET_PROTO(P_DBG,continue, __VA_ARGS__) 
#define ETS(...) ET_PROTO(P_ERR,, __VA_ARGS__) 
#define ETC(...) ET_PROTO(P_ERR,continue, __VA_ARGS__) 
#define ETD(...) ET_PROTO(P_DBG,goto over, __VA_ARGS__) 
#define ET(...) ET_PROTO(P_ERR,goto over, __VA_ARGS__) 

#define E2T_PROTO(ERRTYPE, EXTRA, FUNC, ERRVAL, ERRSTR, ERRVAR, ...) if ((FUNC(__VA_ARGS__)) == ERRVAL) { syslog(ERRTYPE, ERRSTR, ERRVAR); EXTRA; }
#define E2TSD(...) E2T_PROTO(P_DBG,, __VA_ARGS__) 
#define E2TCD(...) E2T_PROTO(P_DBG,continue, __VA_ARGS__) 
#define E2TS(...) E2T_PROTO(P_ERR,, __VA_ARGS__) 
#define E2TC(...) E2T_PROTO(P_ERR,continue, __VA_ARGS__) 
#define E2TD(...) E2T_PROTO(P_DBG,goto over, __VA_ARGS__) 
#define E2T(...) E2T_PROTO(P_ERR,goto over, __VA_ARGS__) 

#define ENT_PROTO(ERRTYPE, EXTRA, FUNC, NERRVAL, ERRSTR, ...) if ((FUNC(__VA_ARGS__)) != NERRVAL) { syslog(ERRTYPE, ERRSTR); EXTRA; }
#define ENTSD(...) ENT_PROTO(P_DBG,, __VA_ARGS__) 
#define ENTCD(...) ENT_PROTO(P_DBG,continue, __VA_ARGS__) 
#define ENTS(...) ENT_PROTO(P_ERR,, __VA_ARGS__) 
#define ENTC(...) ENT_PROTO(P_ERR,continue, __VA_ARGS__) 
#define ENTD(...) ENT_PROTO(P_DBG,goto over, __VA_ARGS__) 
#define ENT(...) ENT_PROTO(P_ERR,goto over, __VA_ARGS__) 

#define E2NT_PROTO(ERRTYPE, EXTRA, FUNC, NERRVAL, ERRSTR, ERRVAR, ...) if ((FUNC(__VA_ARGS__)) != NERRVAL) { syslog(ERRTYPE, ERRSTR, ERRVAR); EXTRA; }
#define E2NTSD(...) E2NT_PROTO(P_DBG,, __VA_ARGS__) 
#define E2NTCD(...) E2NT_PROTO(P_DBG,continue, __VA_ARGS__) 
#define E2NTS(...) E2NT_PROTO(P_ERR,, __VA_ARGS__) 
#define E2NTC(...) E2NT_PROTO(P_ERR,continue, __VA_ARGS__) 
#define E2NTD(...) E2NT_PROTO(P_DBG,goto over, __VA_ARGS__) 
#define E2NT(...) E2NT_PROTO(P_ERR,goto over, __VA_ARGS__) 

#define ENGT_PROTO(ERRTYPE, EXTRA, FUNC, TVAL, ERRSTR, ...) if ((FUNC(__VA_ARGS__)) > TVAL) { syslog(ERRTYPE, ERRSTR); EXTRA; }
#define ENGTSD(...) ENGT_PROTO(P_DBG,, __VA_ARGS__) 
#define ENGTCD(...) ENGT_PROTO(P_DBG,continue, __VA_ARGS__) 
#define ENGTS(...) ENGT_PROTO(P_ERR,, __VA_ARGS__) 
#define ENGTC(...) ENGT_PROTO(P_ERR,continue, __VA_ARGS__) 
#define ENGTD(...) ENGT_PROTO(P_DBG,goto over, __VA_ARGS__) 
#define ENGT(...) ENGT_PROTO(P_ERR,goto over, __VA_ARGS__) 

/* This is a first attempt at the linguistic bizzness */

#define DOAC(ACTION, COMMAND, TO, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); EAC(TO, COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DOASD(ACTION, COMMAND, TO, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); EASD(TO, COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DOAS(ACTION, COMMAND, TO, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); EAS(TO, COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DOAD(ACTION, COMMAND, TO, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); EAD(TO, COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DOA(ACTION, COMMAND, TO, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); EA(TO, COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");

#define DONTC(ACTION, COMMAND, SUCCVAL, ...) syslog(P_DBG, "Trying to " ACTION); ENTC(COMMAND, SUCCVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DONTSD(ACTION, COMMAND, SUCCVAL, ...) syslog(P_DBG, "Trying to " ACTION); ENTSD(COMMAND, SUCCVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DONTS(ACTION, COMMAND, SUCCVAL, ...) syslog(P_DBG, "Trying to " ACTION); ENTS(COMMAND, SUCCVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DONTD(ACTION, COMMAND, SUCCVAL, ...) syslog(P_DBG, "Trying to " ACTION); ENTD(COMMAND, SUCCVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DONT(ACTION, COMMAND, SUCCVAL, ...) syslog(P_DBG, "Trying to " ACTION); ENT(COMMAND, SUCCVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");

#define DOTC(ACTION, COMMAND, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); ETC(COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DOTSD(ACTION, COMMAND, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); ETSD(COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DOTS(ACTION, COMMAND, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); ETS(COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DOTD(ACTION, COMMAND, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); ETD(COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");
#define DOT(ACTION, COMMAND, FAILVAL, ...) syslog(P_DBG, "Trying to " ACTION); ET(COMMAND, FAILVAL, "Cannot " ACTION, __VA_ARGS__); syslog(P_DBG, ACTION " ... done"); syslog(P_DBG, "*-----------------------------------*");

/*****************************************************/

#define P_EMG LOG_DAEMON|LOG_EMERG
#define P_ALT LOG_DAEMON|LOG_ALERT
#define P_CRT LOG_DAEMON|LOG_CRIT
#define P_ERR LOG_DAEMON|LOG_ERR
#define P_WRN LOG_DAEMON|LOG_WARNING
#define P_NTC LOG_DAEMON|LOG_NOTICE
#define P_INF LOG_DAEMON|LOG_INFO
#define P_DBG LOG_DAEMON|LOG_DEBUG

#endif
