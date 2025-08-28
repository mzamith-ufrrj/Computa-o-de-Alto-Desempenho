library(readr)
library(ggplot2)
rm(list = ls())
print("Aceleração e eficiência na multiplicação de matrizes Xeon(R) E-2226G CPU @ 3.40GHz ")
st <- read.table("log-st.csv", header=TRUE, sep= ";")
mt <- read.table("log-mt.csv", header=TRUE, sep= ";")
testes <- read.table("2024-09-14-aceleracao-eficiencia.csv", header=TRUE, sep= ";")
#g <- ggplot(data=testes, aes(x=threads, y=aceleracao, fill=mt$threads)) +
#    geom_bar(stat='summary') + 
#    theme(legend.position="none")
#print(g)
# Use custom colors
#p + scale_fill_manual(values=c('#999999','#E69F00'))
# Use brewer color palettes
#p + scale_fill_brewer(palette="Blues")
g <- ggplot(testes, aes(x=threads, y=eficiencia)) +
  scale_y_continuous(breaks = seq(0, 1, by=0.1), limits=c(0,1.1))  +
  scale_x_continuous(breaks = seq(1, 12, by=1), limits=c(1,12.5))  +
  geom_point(shape=23, fill="blue", color="darkred", size=3)
print(g)