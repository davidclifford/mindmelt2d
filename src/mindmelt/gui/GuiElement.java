package mindmelt.gui;

public class GuiElement {

    private int x;
    private int y;
    private int w;
    private int h;

    public GuiElement(int x, int y, int w, int h) {
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
    }

    public boolean isInside(int x, int y) {
        return (x >= this.x && x <= this.x + w && y >= this.y && y<= this.y+h);
    }
    
}
