complilation command : gcc main.c elgamal_signature.c sha_256.c  padding.c -o main -lgmp

dsa $: ./main
Enter the Message to sign
hello
Message to sign -> hello
_____________________________

randomly generated number alpha : 73226438
P Large prime  -> 118692967
Alpha generator of Z*_p -> 73226438
_____________________________

Private key d -> -> 9568124
Public key Beta -> 59576236
_____________________________

Signed S,R:
108077721 : 63167319 :
_____________________________

Elgamal verify:

alpha^tp mod p -> 102201298
t -> 102201298
Valid Signature

#################################################################################

dsa $: ./main
Enter the Message to sign
Sneha
Message to sign -> Sneha
_____________________________

randomly generated number alpha : 66102351
P Large prime  -> 116258239
Alpha generator of Z*_p -> 66102351
_____________________________

Private key d -> -> 5841028
Public key Beta -> 112106291
_____________________________

Signed Ss:
45847526 :
Unigned ts:
38100005 :
_____________________________

Elgamal verify:

alpha^tp mod p -> 38100005
t -> 38100005
Valid Signature

##################################################################################

dsa $: ./main
Enter the Message to sign
123456789
Message to sign -> 123456789
_____________________________

randomly generated number alpha : 73226438
P Large prime  -> 118692967
Alpha generator of Z*_p -> 73226438
_____________________________

Private key d -> -> 97714907
Public key Beta -> 2450056
_____________________________

Signed S,R:
34333329 : 72109780 :
_____________________________

Elgamal verify:

alpha^tp mod p -> 10228042
t -> 10228042
Valid Signature

#################################################################################

dsa $: ./main
Enter the Message to sign
3421789012225679
Message to sign -> 3421789012225679
_____________________________

randomly generated number alpha : 73226438
P Large prime  -> 118692967
Alpha generator of Z*_p -> 73226438
_____________________________

Private key d -> -> 66521878
Public key Beta -> 28816715
_____________________________

Signed S,R:
115667792 : 29244585 :
_____________________________

Elgamal verify:

alpha^tp mod p -> 114743102
t -> 114743102
Valid Signature
