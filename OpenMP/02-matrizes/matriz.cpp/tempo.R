library(readr)
library(ggplot2)
library(reshape2)
rm(list = ls())
print("Aceleração e eficiência na multiplicação de matrizes Xeon(R) E-2226G CPU @ 3.40GHz ")
#df <- read.table("log-mt.csv", header=TRUE, sep= ";")
#g <- ggplot(df, aes(x=memoria/ 1048576, y=tempo_total)) + geom_point()
#print(g)
t1 <- read.table("log-1.csv", header=TRUE, sep= ";")
t6 <- read.table("log-6.csv", header=TRUE, sep= ";")
t12 <- read.table("log-12.csv", header=TRUE, sep= ";")

g <- ggplot() +
  geom_point(aes(x=t1$memoria / 1048576, y=t1$tempo_total, color="1 thread"), shape=25, size=3) +
  geom_line(aes(x=t1$memoria / 1048576, y=t1$tempo_total, color="1 thread"), size=0.5)+
  geom_point(aes(x=t6$memoria / 1048576, y=t6$tempo_total, color="6 threads"), shape=24, size=3) +
  geom_line(aes(x=t6$memoria / 1048576, y=t6$tempo_total, color="6 threads"), size=0.5)  + 
  geom_point(aes(x=t12$memoria / 1048576, y=t12$tempo_total, color="12 threads"), shape=23, size=3) +
  geom_line(aes(x=t12$memoria / 1048576, y=t12$tempo_total, color="12 threads"), size=0.5)  + 
  xlab(label = 'Memória em MBytes') +
  ylab(label = 'Tempo em segundos') + theme(legend.text = element_text(size=13),
                                     legend.title=element_blank())
print(g)