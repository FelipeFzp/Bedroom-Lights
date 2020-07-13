#include "Arduino.h"
const char pageWiFiConnect[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Configuração - Toikos Detector de Leitos</title>

    <style>
        :root {
            --primary-color: rgb(0, 104, 207);
            --primary-color-light: rgb(14, 129, 243);
            --primary-color-extra-light: rgb(111, 183, 255);
            --primary-color-dark: rgb(0, 63, 126);
            --color-dark: #333;

            /* custom colors */
            --color-turquoise: #1abc9c;
            --color-emerland: #2ecc71;
            --color-peterriver: #3498db;
            --color-amethyst: #9b59b6;
            --color-wetasphalt: #34495e;
            --color-greensea: #16a085;
            --color-nephritis: #27ae60;
            --color-belizehole: #2980b9;
            --color-wisteria: #8e44ad;
            --color-midnightblue: #2c3e50;
            --color-sunflower: #f1c40f;
            --color-carrot: #e67e22;
            --color-alizarin: #e74c3c;
            --color-clouds: #ecf0f1;
            --color-concrete: #95a5a6;
            --color-orange: #f39c12;
            --color-pumpkin: #d35400;
            --color-pomegranate: #c0392b;
            --color-silver: #bdc3c7;
            --color-asbestos: #7f8c8d;
        }

        * {
            font-family: Arial, Helvetica, sans-serif;
        }

        body {
            background-color: var(--primary-color);
            text-align: center;
        }

        .field {
            margin: 13px 0px;
        }

        .field input {
            border-radius: 5px;
            border: 1px #aaa solid;
            padding: 7px 0;
            font-size: 16px;
            width: 100%;
        }

        .field label {
            font-size: 12px;
            font-weight: bold;
            color: var(--color-dark);
        }

        .field button {
            background: var(--primary-color-light);
            color: #fff;
            border: 0;
            padding: 10px;
            border-radius: 5px;
            width: 100%;
            margin: 23px 0 0;
        }

        .title {
            color: var(--primary-color);
        }

        .title h1 {
            font-size: 24px;
        }

        .title h2 {
            font-size: 18px;
            font-weight: 100;
        }

        .box {
            background: #fff;
            margin-left: auto;
            margin-right: auto;
            width: 85%;
            padding: 20px;
            border-radius: 12px;
            margin: 20px auto;
        }

        @media (min-width: 768px) {
            .box {
                width: 50%;
            }
        }

        @media (min-width: 992px) {
            .box {
                width: 30%;
            }
        }
    </style>
</head>

<body>
    <div class="box">
        <div class="title">
            <h1>Toikos Detector de Leitos</h1>
            <h2>Configuração</h2>
        </div>

        <form class="form" id="wifiForm">

            <div class="field">
                <label for="ssid">Nome da rede wi-fi</label>
                <br>
                <input type="text" id="ssid" name="ssid" required>
            </div>

            <div class="field">
                <label for="password">Senha</label>
                <br>
                <input type="password" id="password" name="password" required>
            </div>

            <div class="field">
                <button type="submit">Salvar</button>
            </div>
        </form>

        <br>
        <br>

        <div id="info"></div>
    </div>

    <script>

        const form = document.getElementById('wifiForm');
        const info = document.getElementById('info');

        form.addEventListener('submit', e => {
            e.preventDefault();

            info.innerHTML = '<h3>Aguarde...</h3>';

            fetch("/set-wifi", {
                method: 'POST',
                headers: {
                    "Content-Type": "application/x-www-form-urlencoded"
                },
                body: `ssid=${form.ssid.value}&password=${form.password.value}`
            })
                .then(x => x.text())
                .then(data => {
                    info.innerHTML = `<h3>Conectado</h3><p>IP: ${data}</p>`;
                })
                .catch(err => {
                    info.innerHTML = `<h3>Algo errado</h3><p>${err}</p>`;
                });
        });
    </script>
</body>

</html>

)=====";