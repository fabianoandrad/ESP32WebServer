const btnTeste = document.getElementById("btnTeste");
const mensagem = document.getElementById("mensagem");

async function obterStatus() {

    try {

        const response = await fetch("/api/status");

        if (!response.ok) {
            throw new Error("Erro HTTP: " + response.status);
        }

        const data = await response.json();

        mensagem.textContent =
            `Status: ${data.status} | LED: ${data.led}`;

    }
    catch (error) {

        console.error(error);

        mensagem.textContent =
            "Erro ao conectar com o ESP32";
    }
}

btnTeste.addEventListener("click", obterStatus);
