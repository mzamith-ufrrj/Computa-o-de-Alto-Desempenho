library(readr)
library(ggplot2)
library(reshape2)
rm(list = ls())
print("Aceleração e eficiência na multiplicação de matrizes Xeon(R) E-2226G CPU @ 3.40GHz ")
t1 <- read.table("log-1.csv", header=TRUE, sep= ";")
t2 <- read.table("log-2.csv", header=TRUE, sep= ";")
t3 <- read.table("log-3.csv", header=TRUE, sep= ";")
t4 <- read.table("log-4.csv", header=TRUE, sep= ";")
t5 <- read.table("log-5.csv", header=TRUE, sep= ";")
t6 <- read.table("log-6.csv", header=TRUE, sep= ";")
t7 <- read.table("log-7.csv", header=TRUE, sep= ";")
t8 <- read.table("log-8.csv", header=TRUE, sep= ";")
t9 <- read.table("log-9.csv", header=TRUE, sep= ";")
t10 <- read.table("log-10.csv", header=TRUE, sep= ";")
t11 <- read.table("log-11.csv", header=TRUE, sep= ";")
t12 <- read.table("log-12.csv", header=TRUE, sep= ";")
memoria <- t1$memoria / 1048576

threads = c(2,3,4,5,6)
aceleracao2x <- (t1$tempo_total / t2$tempo_total)
eficiencia2x <- (t1$tempo_total / (2.0 * t2$tempo_total))

aceleracao3x <- (t1$tempo_total / t3$tempo_total)
eficiencia3x <- (t1$tempo_total / (3.0 * t3$tempo_total))

aceleracao4x <- (t1$tempo_total / t4$tempo_total)
eficiencia4x <- (t1$tempo_total / (4.0 * t4$tempo_total))

aceleracao5x <- (t1$tempo_total / t5$tempo_total)
eficiencia5x <- (t1$tempo_total / (5.0 * t5$tempo_total))

aceleracao6x <- (t1$tempo_total / t6$tempo_total)
eficiencia6x <- (t1$tempo_total / (6.0 * t6$tempo_total))

aceleracao7x <- (t1$tempo_total / t7$tempo_total)
eficiencia7x <- (t1$tempo_total / (7.0 * t7$tempo_total))

aceleracao8x <- (t1$tempo_total / t8$tempo_total)
eficiencia8x <- (t1$tempo_total / (8.0 * t8$tempo_total))

aceleracao9x <- (t1$tempo_total / t9$tempo_total)
eficiencia9x <- (t1$tempo_total / (9.0 * t9$tempo_total))

aceleracao10x <- (t1$tempo_total / t10$tempo_total)
eficiencia10x <- (t1$tempo_total / (10 * t10$tempo_total))

aceleracao11x <- (t1$tempo_total / t11$tempo_total)
eficiencia11x <- (t1$tempo_total / (11 * t11$tempo_total))

aceleracao12x <- (t1$tempo_total / t12$tempo_total)
eficiencia12x <- (t1$tempo_total / (12.0 * t12$tempo_total))

#multiplicacao_mat <- data.frame(aceleracao2x, aceleracao3x, aceleracao4x, aceleracao5x, aceleracao6x, eficiencia2x, eficiencia3x, eficiencia4x, eficiencia5x, eficiencia6x)
#g <- ggplot(data=multiplicacao_mat, aes(x=amostras, y=eficiencia2x)) +   geom_bar(stat="identity")  labs(x = "MBytes", y = "Aceleração")
g <- ggplot() +

  #geom_point(aes(x=memoria, y=aceleracao8x), shape=1, fill="deeppink", color="deeppink", size=3) +
  #geom_line(aes(x=memoria, y=aceleracao8x), color="deeppink", size=0.5)+
  geom_point(aes(x=memoria, y=aceleracao7x, color="7 threads"), shape=0, size=3) +
  geom_line(aes(x=memoria, y=aceleracao7x, color="7 threads"), size=0.5)+
  geom_point(aes(x=memoria, y=aceleracao6x, color="6 threads"), shape=21, size=3) +
  geom_line(aes(x=memoria, y=aceleracao6x, color="6 threads"), size=0.5)+
  geom_point(aes(x=memoria, y=aceleracao5x, color="5 threads"), shape=22, size=3) +
  geom_line(aes(x=memoria, y=aceleracao5x, color="5 threads"), size=0.5)+
  geom_point(aes(x=memoria, y=aceleracao4x, color="4 threads"), shape=23, size=3) +
  geom_line(aes(x=memoria, y=aceleracao4x, color="4 threads"), size=0.5)+
  geom_point(aes(x=memoria, y=aceleracao2x, color="2 threads"), shape=24,size=3) +
  geom_line(aes(x=memoria, y=aceleracao2x, color="2 threads"), size=0.5)+
  geom_point(aes(x=memoria, y=aceleracao3x, color="3 threads"), shape=25, size=3) +
  geom_line(aes(x=memoria, y=aceleracao3x, color="3 threads"), size=0.5)  + 
  xlab(label = 'Memória em MBytes') +
  ylab(label = 'Aceleração') + theme(legend.text = element_text(size=13),
                                     legend.title=element_blank())
print(g)

