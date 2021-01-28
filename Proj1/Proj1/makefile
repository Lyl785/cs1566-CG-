LIBS =  -lGLEW -lGLU -lm -lGL

triangle: triangle_ctm.c initShader.o
	gcc -o triangle triangle_ctm.c initShader.o $(LIBS)

initShader.o: initShader.c initShader.h
	gcc -c initShader.c
