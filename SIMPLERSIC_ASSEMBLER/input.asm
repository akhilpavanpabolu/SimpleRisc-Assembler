add r1,r2,r3 #this is first comment
sub r2,r3,44 # this is second comment 
mul r5,r6,r7
div r6,r7,r1
label:
    not r1,r6
    ld r1,r2,r3
b label
st r5,r7,r1
mul r1,r5,r9,r8