this file refers to streets.c and POSIXsem.c
follow in italian

POSIXsem rappresenta il semaforo che regola il traffico
streets  rappresenta la singola strada con la propria coda di macchine

FUNZIONAMENTO POSIXsem:
Per avviare il programma lanciare POSIXsem, questo inizializzerà il semaforo ed avvierà
4 processi figli (streets) dove ognuno rappresenta una strada.
l'output di ogni strada su terminale è rappresentato da un colore diverso.
a questo punto per uscire dal processo inviare un segnale di SIGINT (^C).
l'uscita da questo processo implica anche l'uscita da tutti e 4 i figli.

FUNZIONAMENTO streets:
questo programma deve essere avviato solo come figlio di POSIXsem.
una volta avviato, il programma simula 1 strada con la propria coda di macchine che passano
in base al semaforo gestito da POSIXsem.
Si noti che l'arrivo delle macchine non rispetta nessuna particolare distribuzione ma 
è semplicemente gestito da una variabile generata casualmente.

per maggiori dettagli vedere i sorgenti POSIXsem.c e streets.c