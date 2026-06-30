# VoiceToBraille
Takes audio input and gives CAM_follower servo motor commands to rotate :-
The voiceToTextmain.py file uses google speech recognition API and converts text to speech live time
Then we send the text to textTobraille.py file which takes in text (output from voiceToTextMain file) and converts text into many hot encodings representing which cam follower pin should move when
at last we send the encodings to brailleToangle.cpp file which takes input as the encodings and then converts it into angles by which the servo motor then rotates 
