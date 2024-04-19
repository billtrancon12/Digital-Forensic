CFLAGS= -Doff64_t=__off64_t

all: printFileName printDataAttribute recoverFile blockCount

blockCount: blockCount.c getPartition.o utility.o
	gcc ${CFLAGS}  -o blockCount blockCount.c getPartition.o utility.o -lm

recoverFile: recoverFile.c utility.o getMftAddr.o getPartition.o listAttribute.o
	gcc ${CFLAGS} -o recoverFile recoverFile.c getMftAddr.o utility.o getPartition.o listAttribute.o

printFileName:  printFileName.c utility.o getMftAddr.o getPartition.o listAttribute.o
	gcc ${CFLAGS} -o printFileName printFileName.c utility.o getPartition.o getMftAddr.o listAttribute.o

printDataAttribute: printDataAttribute.c utility.o getMftAddr.o getPartition.o listAttribute.o
	gcc ${CFLAGS} -o printDataAttribute printDataAttribute.c utility.o getPartition.o getMftAddr.o listAttribute.o

listAttribute.o: listAttribute.c utility.h attrHeader.h
	gcc ${CFLAGS} -c listAttribute.c 

utility.o: utility.c utility.h
	gcc ${CFLAGS} -c utility.c

getMftAddr.o: getMftAddr.c br.h getMftAddr.h mbr.h
	gcc ${CFLAGS} -c getMftAddr.c

getPartition.o: getPartition.c getPartition.h mbr.h
	gcc ${CFLAGS} -c getPartition.c


#target: listAttribute.c getMftAddr.o getPartition.c utility.o
#	gcc ${CFLAGS} -o listAttribute listAttribute.c getMftAddr.o getPartition.c utility.o

clean: 
	$(RM)  *.o *~
