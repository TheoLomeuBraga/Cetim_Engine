const main_frame = document.getElementById('main_frame');
main_frame.src = "./pages/hello.html";

class Button_Linker{
    constructor(id,page){
        this.id = id
        this.page = page
    }
}
buttons_linkers = [
    new Button_Linker("hello","./pages/hello.html"),
]