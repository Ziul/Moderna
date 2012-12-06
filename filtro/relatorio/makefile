# É só escrever o comando "make". Entro com "make clean" para limpar a sujeira e
# "make buildclean" para deletar o pdf

all: main.pdf backup

# Compila a cada alteração de qualquer arquivo *.tex ou de qualquer *.vhd dentro da pasta 'src'
main.pdf: *.tex *.bib clean
	clear
#	pdflatex -interaction errorstopmode -interaction=batchmode main.tex
	pdflatex main.tex
	clear
	@echo "Compilado pela primeira vez...Feito."
	@echo -n "Montando bibliografias..."
	bibtex main -terse
	@echo "Feito."
	@echo "Compilando pela segunda vez:"
	@pdflatex -interaction=batchmode main.tex
	@echo -n "Feito\nCompilando pela ultima vez:\n"
	@pdflatex -interaction=batchmode main.tex
	@echo -n "Limpando sujeira..."
	@make clean
	@echo "Feito."
	@make backup
	@echo "Informações do arquivo gerado:"
	pdfinfo main_final.pdf
	
# Limpa qualquer sujeira que reste após compilação
# Útil que objetos de linguagens são incluidos e ficam relatando erros após retirados.
clean:
	rm -rf *.aux *.log *.toc *.bbl *.bak *.blg *.out *.lof *.lot
	
buildclean:
	rm -rf main.pdf
	
# Por algum motivo o *.pdf sumia da pasta. Gerado apenas para guardar uma copia de segurança na pasta
backup: main.pdf
	pdfopt main.pdf main_final.pdf
	
.SILENT:
