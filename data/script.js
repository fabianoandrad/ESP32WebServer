const btnTeste = document.getElementById("btnTeste");
const mensagem = document.getElementById("mensagem");

btnTeste.addEventListener("click", () => {

    mensagem.textContent = "JavaScript funcionando no ESP32!";

});
