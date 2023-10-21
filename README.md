# carrinho_esp32
remote control car using ESP32

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
