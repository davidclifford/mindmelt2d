package mindmelt.gui;

public class Button extends GuiElement {
    
    public static final int off = 1;
    public static final int up = 2;
    public static final int down = 3;
    
    public static final int buttonSize = 32;
    
    private int inactive_tile;
    private int active_tile;
    
    private int state = off;
    private int subcode = 0;

    public Button(int x, int y, int w, int h) {
        super(x, y, buttonSize, buttonSize);
    }

    public int getState() {
        return state;
    }

    public void setState(int state) {
        this.state = state;
    }

    public int getSubcode() {
        return subcode;
    }

    public void setSubcode(int subcode) {
        this.subcode = subcode;
    }

    public void setInactive_tile(int inactive_tile) {
        this.inactive_tile = inactive_tile;
    }

    public void setActive_tile(int active_tile) {
        this.active_tile = active_tile;
    }
    
    public String toString() {
        return String.format("x[%d] y[%d]", x, y );
    }
    
    public void click(int x, int y) {
        if (state == off) return;
        if (state == up) state = down;     
    }
}
