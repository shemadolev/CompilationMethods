COPYI I1 0
COPYI I2 0
COPYI I3 0
COPYI I4 0
COPYI I5 0
COPYI I6 0
COPYI I7 0
JLINK 84
HALT
ADD2I I2 I2 8
ADD2I I2 I2 4
LOADI I5 I4 4
COPYI I6 0
SEQUI I7 I5 I6
BNEQZ I7 17
UJUMP 20
COPYI I8 1
COPYI I3 I8
RETRN
LOADI I9 I4 4
COPYI I10 1
SEQUI I11 I9 I10
BNEQZ I11 25
UJUMP 28
LOADI I12 I4 0
COPYI I3 I12
RETRN
ADD2I I13 I4 8
LOADI I14 I4 0
LOADI I15 I4 4
COPYI I16 1
SUBTI I17 I15 I16
STORI I5 I2 0
STORI I6 I2 4
STORI I7 I2 8
STORI I8 I2 12
STORI I9 I2 16
STORI I10 I2 20
STORI I11 I2 24
STORI I12 I2 28
STORI I13 I2 32
STORI I14 I2 36
STORI I15 I2 40
STORI I16 I2 44
STORI I17 I2 48
ADD2I I2 I2 52
STORI I0 I2 0
STORI I4 I2 4
STORI I1 I2 8
ADD2I I1 I2 8
STORI I14 I1 4
STORI I17 I1 8
ADD2I I4 I1 4
ADD2I I2 I1 12
JLINK 10
COPYI I2 I1
LOADI I1 I1 0
SUBTI I2 I2 8
LOADI I4 I2 4
LOADI I0 I2 0
SUBTI I2 I2 52
LOADI I5 I2 0
LOADI I6 I2 4
LOADI I7 I2 8
LOADI I8 I2 12
LOADI I9 I2 16
LOADI I10 I2 20
LOADI I11 I2 24
LOADI I12 I2 28
LOADI I13 I2 32
LOADI I14 I2 36
LOADI I15 I2 40
LOADI I16 I2 44
LOADI I17 I2 48
COPYI I18 I3
STORI I18 I13 0
LOADI I19 I4 8
LOADI I20 I4 0
MULTI I21 I19 I20
COPYI I3 I21
RETRN
SUBTI I2 I2 12
RETRN
ADD2I I2 I2 4
ADD2I I2 I2 4
ADD2I I2 I2 4
ADD2I I5 I4 0
READI I6
STORI I6 I5 0
ADD2I I7 I4 8
READI I8
STORI I8 I7 0
ADD2I I9 I4 4
LOADI I10 I4 0
LOADI I11 I4 8
STORI I5 I2 0
STORI I6 I2 4
STORI I7 I2 8
STORI I8 I2 12
STORI I9 I2 16
STORI I10 I2 20
STORI I11 I2 24
ADD2I I2 I2 28
STORI I0 I2 0
STORI I4 I2 4
STORI I1 I2 8
ADD2I I1 I2 8
STORI I10 I1 4
STORI I11 I1 8
ADD2I I4 I1 4
ADD2I I2 I1 12
JLINK 10
COPYI I2 I1
LOADI I1 I1 0
SUBTI I2 I2 8
LOADI I4 I2 4
LOADI I0 I2 0
SUBTI I2 I2 28
LOADI I5 I2 0
LOADI I6 I2 4
LOADI I7 I2 8
LOADI I8 I2 12
LOADI I9 I2 16
LOADI I10 I2 20
LOADI I11 I2 24
COPYI I12 I3
STORI I12 I9 0
LOADI I13 I4 4
PRNTI I13
SUBTI I2 I2 12
RETRN
HALT
