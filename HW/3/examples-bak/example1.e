COPYI I1 0
COPYI I2 0
COPYI I3 0
COPYI I4 0
COPYI I5 0
COPYI I6 0
COPYI I7 0
JLINK 10
HALT
ADD2I I2 I2 4
ADD2I I2 I2 4
ADD2I I3 I1 0
READI I4
STORI I4 I3 0
ADD2I I5 I1 4
COPYI I6 0
STORI I6 I5 0
LOADI I7 I1 0
COPYI I8 0
SNEQI I9 I7 I8
BREQZ I9 34
UJUMP 23
ADD2I I10 I1 0
LOADI I11 I1 0
COPYI I12 10
DIVDI I13 I11 I12
STORI I13 I10 0
ADD2I I14 I1 4
LOADI I15 I1 4
COPYI I16 1
ADD2I I17 I15 I16
STORI I17 I14 0
UJUMP 18
PRNTC 34
PRNTC 97
PRNTC 34
PRNTC 32
PRNTC 104
PRNTC 97
PRNTC 115
PRNTC 32
LOADI I18 I1 4
PRNTI I18
PRNTC 32
PRNTC 100
PRNTC 105
PRNTC 103
PRNTC 105
PRNTC 116
PRNTC 115
PRNTC 10
RETRN