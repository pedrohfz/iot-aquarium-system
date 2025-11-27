# 🐠 Monitoramento Inteligente de Aquários com IoT

    Monitoramento em Tempo Real • Alertas via WhatsApp • Dashboard Grafana • Wokwi + ESP32 + MQTT + Node-RED + InfluxDB

## 📌 Resumo do Projeto

    Este projeto implementa um sistema completo de monitoramento ambiental para aquários, utilizando tecnologias IoT modernas. Dois tanques são monitorados em tempo real quanto aos parâmetros críticos:

    - Temperature;
    - pH.

    Os dados são recebidos por duas ESP32 (simuladas no Wokwi) e enviados via MQTT para o Node-RED, onde são processados, armazenados no InfluxDB e exibidos em um dashboard Grafana. Caso alguma leitura esteja fora dos limites, um alerta automático via WhatsApp é enviado.

## 🔧 Tecnologias Utilizadas

    1. Hardware
    - ESP32 DevKit V1;
    - Potenciômetros simulando temperatura e pH.

    2. Software & Serviços
    - Wokwi — simulação do hardware;
    - MQTT (HiveMQ) — comunicação IoT;
    - Node-RED — integração e automação;
    - InfluxDB — banco de dados de séries temporais;
    - Grafana — dashboard visual;
    - CallMeBOT API — envio de alertas via WhatsApp.

## 📡 Fluxo de Funcionamento

    1. Coleta de Dados
    Cada ESP32 lê dois sensores simulados (temp + pH) por meio de potenciômetros.

    2. Publicação MQTT
    Os dados são enviados como JSON:

    {
    "ts": "2025-11-17T20:18:27Z",
    "aquario": "tanqueA",
    "temp_c": 23.6,
    "ph": 5.19
    }

    3. Node-RED
    - Converte JSON → objeto;
    - Verifica limites;
    - Envia tudo para o InfluxDB;
    - Envia alerta via WhatsApp quando necessário.

    4. InfluxDB + Grafana
    - Armazena histórico completo;
    - Dashboard mostra gráficos, cards e tabela de últimas leituras.

## 🧠 Conclusão

    O sistema se mostrou eficiente e confiável para monitoramento ambiental de aquários.
    A arquitetura modular permite:
    - Escalar para dezenas de tanques;
    - Adicionar sensores reais;
    - Automatizar bombas, aquecedores e sistemas de pH;
    - Criar relatórios automáticos;
    - Integrar soluções de IA para previsão de falhas;
    - O objetivo do projeto foi totalmente atingido.

📝 Autores

    Pedro Henrique Leite, João Victor Cara Santos, Isadora Caetano Brandão de Sousa e Letícia Santiago da Silva
    Estudante de Sistemas de Informação — Universidade Presbiteriana Mackenzie