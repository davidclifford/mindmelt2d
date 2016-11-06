package mindmelt.gui;

public class GuiElement {

    protected int x;
    protected int y;
    protected int w;
    protected int h;
    
    protected GuiElement parent = null;

    public GuiElement(int x, int y, int w, int h) {
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
    }

    public GuiElement getParent() {
        return parent;
    }

    public void setParent(GuiElement parent) {
        this.parent = parent;
    }
    
    public boolean isInside(int x, int y) {
        return (x >= this.x && x <= this.x + w && y >= this.y && y<= this.y+h);
    }
    
    public void click(int x, int y) {
        System.out.println(this.toString());
    }
    
}
