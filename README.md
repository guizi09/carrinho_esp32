# carrinho_esp32
Projeto usado para a disciplina de eletrônica digital avançada da PUC MINAS


# Mapa do Diretório
```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
├── image
│   ├── esp32.png
├── database
│   ├── 
│     
```

# Componentes Utilizados
Os componentes usados para a construção do carrinho foram:

| Componente | Função |
|-------------|-------------|
| Ponte H L298N | Controlar os motores | 
| Motor DC 3-6V | Usado para movimentar as rodas | 
| Roda 68mm | Conectadas ao Motor DC |
| ESP32 | Microcontrolador do sistema |
| Controle de PS3 | Usado para controlar o carrinho |


# Funcionamento dos moto

Na PONTE H L298 é possivel controlar a rotação do motor colando as seguintes entradas logícas:

| Motor A | IN1 | IN2 |
|-------------|-------------|-------------|
| SENTIDO DIRETO | HIGH | LOW |
| SENTIDO REVERSO | LOW | HIGH |
| FREIO | LOW | LOW |
| FREIO | HIGH | HIGH |

| Motor B | IN3 | IN4 |
|-------------|-------------|-------------|
| SENTIDO DIRETO | HIGH | LOW |
| SENTIDO REVERSO | LOW | HIGH |
| FREIO | LOW | LOW |
| FREIO | HIGH | HIGH |

# Links Uteis
Utilização da PONTE H L298N: https://blog.eletrogate.com/guia-definitivo-de-uso-da-ponte-h-l298n/
