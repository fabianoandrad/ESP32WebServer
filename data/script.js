const btnTeste = document.getElementById("btnTeste");
const mensagem = document.getElementById("mensagem");

let ledState = false;

// =============================
// GET STATUS
// =============================
async function getStatus() {
    try {
        const response = await fetch("/api/status");

        if (!response.ok) {
            throw new Error("Erro HTTP: " + response.status);
        }

        const data = await response.json();

        console.log("Resposta getStatus do ESP32:", data);

        btnTeste.textContent = data.led === "ligado" ? "Desligar LED" : "Ligar LED";

        mensagem.textContent = `Status: ${data.status} | LED: ${data.led}`;
    }
    catch (error) {
        console.error(error);
        mensagem.textContent = "Erro ao conectar com o ESP32";
    }
}

// =============================
// POST LED STATE
// =============================
async function postLedState() {
    // Inverte o estado atual
    const newState = !ledState;

    console.log("Enviando:", newState);

    try {
        const response = await fetch("/api/ledState", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({
                state: newState
            })
        });

        if (!response.ok) {
            throw new Error("Erro HTTP: " + response.status);
        }

        const data = await response.json();

        console.log("Resposta postLedState do ESP32:", data);

        // Só atualiza depois que o ESP32 confirmou
        ledState = newState;

        // Consulta novamente o estado real
        await getStatus();

    }
    catch (error) {
        console.error(error);
        mensagem.textContent = "Erro ao alterar o estado do LED";
    }
}

// =============================
// EVENTO DO BOTÃO
// =============================
btnTeste.addEventListener("click", postLedState);

// =============================
// INICIALIZAÇÃO
// =============================
getStatus();
