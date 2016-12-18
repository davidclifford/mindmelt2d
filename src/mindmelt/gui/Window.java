package mindmelt.gui;

import java.util.ArrayList;
import java.util.List;
import org.newdawn.slick.Image;
import org.newdawn.slick.TrueTypeFont;

public class Window extends GuiElement {
    private int x;
    private int y;
    private int w;
    private int h;
    
    private List<Zone> zones = new ArrayList<>();
    private List<Button> buttons = new ArrayList<>();

    public Window(Image tiles, TrueTypeFont ttf, int x, int y, int w, int h) {
        super(tiles,ttf,x*SC,y*SC,w*SC,h*SC);
    }
    
    public Window addZone(Zone zone) {
        zones.add(zone);
        zone.setParent(this);
        return this;
    }

    public Window addButton(Button button) {
        buttons.add(button);
        button.setParent(this);
        return this;
    }
    
    public void click(int x, int y)
    {
        GuiElement elem = getElement(x, y);
        if (elem==null) {
            return;
        }
        int xx = x-this.x;
        int yy = y-this.y;
        elem.click(xx, yy);
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
