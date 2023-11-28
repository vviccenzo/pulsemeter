
<h1>Sensor PulseMeter</h1>

<p>O <strong>Sensor PulseMeter</strong> é um projeto que utiliza o ESP32 para monitorar os batimentos cardíacos e enviar notificações por email e WhatsApp quando detecta um aumento significativo nos batimentos cardíacos do usuário. O dispositivo se conecta à rede Wi-Fi para enviar as notificações.</p>

<h2>Conteúdo do Projeto</h2>

<h3>Arquivos Principais</h3>

<ul>
    <li><strong>PulseMeter_ESP32.ino:</strong> Este é o arquivo principal do projeto, contendo o código-fonte em Arduino. Ele configura a conexão Wi-Fi, inicializa o cliente SMTP para enviar emails e contém a lógica principal para detectar e notificar sobre batimentos cardíacos elevados.</li>
</ul>

<h3>Dependências</h3>

<ul>
    <li><strong>ESP_Mail_Client.h:</strong> Biblioteca que fornece suporte para envio de emails via SMTP usando o ESP32.</li>
</ul>

<h3>Configurações</h3>

<p>As configurações do projeto estão no início do arquivo PulseMeter_ESP32.ino:</p>

<ul>
    <li><strong>WIFI_SSID:</strong> Nome da rede Wi-Fi à qual o dispositivo se conectará.</li>
    <li><strong>WIFI_PASSWORD:</strong> Senha da rede Wi-Fi.</li>
    <li><strong>SMTP_HOST:</strong> Host do servidor SMTP para envio de emails (configurado para o Office 365 neste exemplo).</li>
    <li><strong>SMTP_PORT:</strong> Porta do servidor SMTP (configurada para a porta 587 neste exemplo).</li>
    <li><strong>AUTHOR_EMAIL:</strong> Endereço de email do remetente.</li>
    <li><strong>AUTHOR_PASSWORD:</strong> Senha do email do remetente.</li>
    <li><strong>config.server.user_domain:</strong> Domínio do usuário (exemplo: mydomain.net).</li>
    <li><strong>config.time:</strong> Configurações relacionadas ao servidor de tempo NTP.</li>
</ul>

<h3>Funcionalidades Principais</h3>

<ul>
    <li><strong>Conexão Wi-Fi:</strong> O dispositivo se conecta à rede Wi-Fi configurada.</li>
    <li><strong>Envio de Email:</strong> Utiliza a biblioteca ESP_Mail_Client para enviar emails quando detecta batimentos cardíacos elevados.</li>
    <li><strong>Configuração do Email:</strong> Configura o remetente, destinatário, assunto e corpo do email.</li>
</ul>

<h2>Configuração do Projeto</h2>

<ol>
    <li><strong>Configuração Wi-Fi:</strong> Substitua as variáveis WIFI_SSID e WIFI_PASSWORD com o nome e senha da sua rede Wi-Fi.</li>
    <pre>
<code>#define WIFI_SSID "SuaRedeWiFi"</code>
<code>#define WIFI_PASSWORD "SuaSenhaWiFi"</code>
    </pre>
    <li><strong>Configuração do Servidor SMTP:</strong> Atualize as variáveis SMTP_HOST, SMTP_PORT, AUTHOR_EMAIL e AUTHOR_PASSWORD com as informações do seu servidor de email.</li>
    <pre>
<code>#define SMTP_HOST "smtp.seuservidor.com"</code>
<code>#define SMTP_PORT esp_mail_smtp_port_587</code>
<code>#define AUTHOR_EMAIL "seuemail@dominio.com"</code>
<code>#define AUTHOR_PASSWORD "suaSenhaEmail"</code>
    </pre>
    <li><strong>Configuração do Domínio do Usuário:</strong></li>
    <pre>
<code>config.login.user_domain = F("mydomain.net");</code>
    </pre>
    <li><strong>Configuração do Tempo NTP:</strong> Personalize as configurações de tempo de acordo com sua localização.</li>
    <pre>
<code>config.time.ntp_server = F("pool.ntp.org,time.nist.gov");</code>
<code>config.time.gmt_offset = 0;</code>
<code>config.time.day_light_offset = 0;</code>
<code>config.time.timezone_env_string = "&lt;-03&gt;3";</code>
    </pre>
    <li><strong>Configuração do Email de Notificação:</strong> Modifique as variáveis assunto e textMsg para personalizar o conteúdo do email de notificação.</li>
    <pre>
<code>String assunto =  "AJUDA! O portador do medidor de batimentos está em risco!";</code>
<code>String textMsg = "A mente que se abre a uma nova ideia jamais voltará ao seu tamanho original.'' Albert Einstein";</code>
    </pre>
</ol>

<h2>Compilação e Upload</h2>

<p>Compile o código usando a IDE do Arduino e faça o upload para o seu ESP32. Certifique-se de ter as bibliotecas necessárias instaladas.</p>

<h2>Notas Adicionais</h2>

<ul>
    <li><strong>Reconexão Automática:</strong> O código implementa uma reconexão automática à rede Wi-Fi caso a conexão seja perdida.</li>
    <li><strong>Logs de Email:</strong> O código exibe logs detalhados do processo de envio de email via porta serial.</li>
</ul>

<p>Este projeto fornece uma base para a criação de um dispositivo simples de monitoramento cardíaco com notificações automáticas. Personalize conforme necessário para atender aos requisitos específicos do seu projeto.</p>
