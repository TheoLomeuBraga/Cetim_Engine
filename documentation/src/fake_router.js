const main_frame = document.getElementById('main_frame');
main_frame.src = "./pages/hello.html";

const meuMap = new Map();
meuMap.set("hello", "./pages/hello.html");
meuMap.set("guide_lines", "./pages/guide_lines.html");

meuMap.forEach(function (valor, chave) {
    const elemento = document.getElementById(chave);
    if (elemento) {
        elemento.addEventListener("click", function () {
            main_frame.src = valor;
        });
    } else {
        console.error("Elemento com ID " + chave + " não encontrado.");
    }
});

