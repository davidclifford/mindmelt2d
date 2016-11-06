package mindmelt.gui;

public class WindowSystem extends GuiElement {

    private Window window;
    
    public WindowSystem(Window window, int x, int y, int w, int h) {
        super(x, y, w, h);
        this.window = window;
    }

    public Window getWindow() {
        return window;
    }

    public void setWindow(Window window) {
        this.window = window;
    }
   
    public void draw() {
    }
}
