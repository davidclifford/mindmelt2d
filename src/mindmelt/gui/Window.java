package mindmelt.gui;

import java.util.ArrayList;
import java.util.List;

public class Window extends GuiElement {
    private int x;
    private int y;
    private int w;
    private int h;
    
    private List<Zone> zones = new ArrayList<>();
    private List<Button> buttons = new ArrayList<>();

    public Window(int x, int y, int w, int h) {
        super(x,y,w,h);
    }
    
    public Window addZone(Zone zone) {
        zones.add(zone);
        return this;
    }

    public Window addButton(Button button) {
        buttons.add(button);
        return this;
    }
   
    public GuiElement getElement(int x, int y)
    {
        int xx = x-this.x;
        int yy = y-this.y;
        for(Button but:buttons) {
            if (but.isInside(xx,yy)) {
                return but;
            }
        }
        for(Zone zon:zones) {
            if (zon.isInside(xx,yy)) {
                return zon;
            }
        }
        return this;
    }

    public int getXPosition() {
        return x;
    }

    public int getYPosition() {
        return y;
    }

    public int getWidth() {
        return w;
    }

    public int getHeight() {
        return h;
    }
}
