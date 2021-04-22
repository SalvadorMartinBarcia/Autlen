CC = gcc
CFLAGS = -g -ansi -Wall -pedantic
CFLAGS2 = -g
EXE = pr_afnd
#los ejecutables que vayas a crear
# $@ es lo que está a la izq de :
# $< es el primer item en la lista de dependencias
# $^ es la lista de items a la derecha del :

all: $(EXE) clear

$(EXE): % : %.o afnd_ofus.o minimiza.o transforma.o funciones.o stack_int.o #los .o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $^


pr_afnd.o: pr_afnd.c transforma.h funciones.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

transforma.o: transforma.c afnd.h funciones.h stack_int.h transforma.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

minimiza.o: minimiza.c minimiza.h afnd.h funciones.h stack_int.h transforma.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

funciones.o: funciones.c afnd.h funciones.h transforma.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

afnd_ofus.o: afnd_ofus.c afnd.h transforma.h funciones.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS2) -c $<

stack_int.o: stack_int.c transforma.h funciones.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<


valgrind:
	valgrind --leak-check=full ./pr_afnd

clean :
	rm -f *.o core $(EXE) *.dot *.png

clear:
	rm -f *.o

dot:
	dot afdm.dot -Tpng -o afdm.png
