# Prática 4: Integração IoT com Blynk e FreeRTOS (ESP32) 🌐📲
### Este repositório apresenta o desenvolvimento de um **Nó de Monitoramento IoT**, integrando aquisição de dados em tempo real, interface local (LCD) e telemetria em nuvem via plataforma Blynk.

## 🎯 Objetivos Didáticos
1. **Conectividade IoT:** Implementação de comunicação **WiFi** e integração com a biblioteca **Blynk**.

2. **Sincronização Nuvem-Local:** Uso de **Mutexes** para garantir que a atualização dos dados no LCD e o envio para os pinos virtuais do Blynk (`Blynk.virtualWrite`) ocorram de forma atômica e segura.

3. **Controle Atuado via Cloud:** Uso da macro `BLYNK_WRITE(V0)` para receber comandos da nuvem e atuar no hardware local (LED), demonstrando controle bidirecional.

## ⚙️ Arquitetura do Sistema
O sistema utiliza a arquitetura Dual-Core do ESP32 para separar as responsabilidades:

* **Core 0 (TaskSlider):** Monitora o potenciômetro e envia para o pino virtual **V1**.

* **Core 1 (TaskTemp):** Monitora o sensor DHT22 e envia para o pino virtual **V2**.

* **Loop Principal:** Gerencia a pilha de protocolos do Blynk e a manutenção da conexão WiFi.

## ⚠️ Nota Técnica sobre P&D
A integração de `Blynk.virtualWrite` dentro de seções críticas protegidas por semáforos é uma prática recomendada para evitar conflitos de barramento quando múltiplos núcleos tentam acessar a pilha de rede ou periféricos de saída simultaneamente.

## 🚀 Como Executar
1. O código principal está localizado no arquivo sketch.ino.
2. As conexões de hardware estão descritas no arquivo diagram.json.
3. As bibliotecas estão especificadas no arquivo libraries.txt 
4. **Simulação Online:** Você pode testar o comportamento do sistema diretamente no navegador:
    * ▶️ [Link para a Simulação no Wokwi](https://wokwi.com/projects/436019129502895105) 

## ⚠️ Nota sobre Acesso ao Dashboard:
Por questões de segurança e integridade das credenciais da plataforma **Blynk (Auth Token e Template ID)**, o acesso ao dashboard interativo é restrito ao desenvolvedor. No entanto, o funcionamento da telemetria e o controle bidirecional podem ser verificados através do vídeo demonstrativo e dos logs no Serial Monitor incluídos neste repositório.

---
### Prof. Thiago José da Luz, PhD 