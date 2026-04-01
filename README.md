# Prática 4: Integração IoT com Blynk e FreeRTOS (ESP32) 🌐📲

Este repositório apresenta o desenvolvimento da **Prática 4** da disciplina de **Sistemas Operacionais Embarcados**, ministrada no **Departamento de Engenharia Elétrica da Universidade Federal do Paraná (UFPR)**.  
O projeto consiste na implementação de um **Nó de Monitoramento IoT**, integrando **aquisição de dados em tempo real**, **interface local (LCD)** e **telemetria em nuvem**, utilizando a plataforma **Blynk** em conjunto com **FreeRTOS no ESP32**.

Esta prática estende os conceitos de multitarefa, sincronização e exclusão mútua para um **ambiente conectado à nuvem**, aproximando-se de aplicações reais de **IoT industrial e sistemas ciber-físicos**.

---

## 🎯 Objetivos Didáticos

1. **Conectividade IoT:**  
   Implementar comunicação **WiFi** e integração com a plataforma **Blynk**, permitindo telemetria remota dos dados.

2. **Sincronização Nuvem–Local:**  
   Utilizar **Mutexes** para garantir que a atualização do display LCD local e o envio simultâneo de dados aos pinos virtuais do Blynk (`Blynk.virtualWrite`) ocorram de forma **atômica, segura e consistente**.

3. **Controle Atuado via Cloud:**  
   Implementar controle bidirecional utilizando a macro `BLYNK_WRITE(V0)`, permitindo comandos enviados da nuvem para atuação direta no hardware local (LED).

---

## 🧠 Competências Demonstradas

- Integração de sistemas embarcados com **plataformas IoT em nuvem**
- Programação multitarefa com **FreeRTOS** em arquitetura **dual-core**
- Uso de **Mutexes** para sincronização entre tarefas locais e serviços de rede
- Comunicação bidirecional **cloud ↔ edge**
- Projeto de sistemas embarcados conectados (IoT Nodes)
- Integração segura entre **interfaces locais e telemetria remota**
- Análise de concorrência envolvendo **pilha de rede e periféricos físicos**

---

## ⚙️ Arquitetura do Sistema

O sistema explora a arquitetura **Dual-Core do ESP32**, separando responsabilidades para melhorar organização e desempenho:

- **Core 0 – TaskSlider:**  
  Leitura contínua do potenciômetro e envio do valor para o pino virtual **V1** no Blynk.

- **Core 1 – TaskTemp:**  
  Leitura do sensor **DHT22** e envio dos dados de temperatura para o pino virtual **V2** no Blynk.

- **Loop Principal:**  
  Responsável pela manutenção da conexão WiFi, gerenciamento da pilha de protocolos e execução do `Blynk.run()`.

A sincronização entre as tarefas ocorre por meio de **Mutexes**, garantindo acesso exclusivo ao LCD e às rotinas de envio de dados.

---

## 🛠️ Hardware Utilizado (Simulado no Wokwi)

- **Microcontrolador:** ESP32  
- **Sensores:**  
  - DHT22 (Temperatura)  
  - Potenciômetro (Entrada analógica)  
- **Display:** LCD 16x2  
- **Interface IoT:** Plataforma Blynk (WiFi)

---

## 💡 Relevância Técnica

A integração segura entre tarefas locais e serviços de nuvem é um desafio recorrente em **sistemas IoT reais**, especialmente quando múltiplos núcleos acessam simultaneamente periféricos físicos e a pilha de rede.  
Esta prática demonstra uma abordagem robusta para:

- evitar conflitos de barramento;
- garantir consistência entre dados locais e remotos;
- preservar a estabilidade do sistema em arquiteturas multitarefa.

O uso de **Mutexes envolvendo chamadas a `Blynk.virtualWrite`** reflete boas práticas adotadas em projetos profissionais de **IoT industrial, smart monitoring e edge computing**.

---

## 🚀 Como Executar

1. O código principal está localizado no arquivo `sketch.ino`.
2. As conexões de hardware estão descritas no arquivo `diagram.json`.
3. As bibliotecas necessárias estão listadas no arquivo `libraries.txt`.
4. **Simulação Online:**  
   O comportamento do sistema pode ser testado diretamente no navegador por meio do link abaixo:  
   ▶️ [Link para a Simulação no Wokwi](https://wokwi.com/projects/436019129502895105)

---

## 🧪 Validação e Testes

- Verificação da atualização consistente dos dados no display LCD.
- Monitoramento da telemetria em tempo real no dashboard do Blynk.
- Teste de envio simultâneo de dados por múltiplas tarefas.
- Avaliação do controle bidirecional via pino virtual **V0**.
- Confirmação da ausência de conflitos entre acesso à rede e periféricos locais.

---

## ⚠️ Nota sobre Acesso ao Dashboard

Por motivos de **segurança e proteção de credenciais** da plataforma **Blynk**  
(Auth Token e Template ID), o acesso direto ao dashboard interativo é restrito ao desenvolvedor.  

Entretanto, o funcionamento da telemetria e do controle bidirecional pode ser verificado por meio:
- dos logs no **Serial Monitor**;
- do vídeo demonstrativo no [linkedin](https://www.linkedin.com/posts/thiago-jos%C3%A9-da-luz-ba2ba3123_sistemasembarcados-freertos-iot-activity-7443752555232514048-5GnL?utm_source=share&utm_medium=member_desktop&rcm=ACoAAB6cOOoBf0lqu7Zx9u1EC4q7Qn-yIoyeISw)

---

## 📚 Evolução do Projeto

Com esta prática, o portfólio avança para **sistemas embarcados conectados**, consolidando os conceitos abordados anteriormente:

- **Prática 1:** Sistemas reativos com interrupções (bare-metal)  
- **Prática 2:** Concorrência e condição de corrida  
- **Prática 3:** Sincronização com Mutex  
- **Prática 4:** Integração IoT com concorrência segura  

Como próximos passos, o portfólio pode explorar:
- Filas (Queues) para desacoplamento entre aquisição e envio de dados;
- MQTT para comunicação entre dois ESP32.

---

**Prof. Thiago José da Luz, PhD**  
Departamento de Engenharia Elétrica – UFPR
