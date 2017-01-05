package mindmelt.gui;

import java.util.ArrayList;
import java.util.List;
import mindmelt.action.Action;
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
        super(tiles,ttf,x,y,w,h);
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
    
    public void click(int x, int y, int button)
    {
        int xx = x-this.x;
        int yy = y-this.y;        
        GuiElement elem = getElement(xx, yy);
        if (elem!=null) {
            elem.click(xx, yy, button);
        }
    }
   
    public GuiElement getElement(int x, int y)
    {
        for(Button but:buttons) {
            if (but.isInside(x,y)) {
                return but;
            }
        }
        for(Zone zon:zones) {
            if (zon.isInside(x,y)) {
                return zon;
            }
        }
        return null;
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
