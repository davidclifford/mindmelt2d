package mindmelt.gui;

import mindmelt.action.Action;
import org.newdawn.slick.Color;
import org.newdawn.slick.Image;
import org.newdawn.slick.TrueTypeFont;

public class GuiElement {

    public static final int SC = 32;
    protected int x;
    protected int y;
    protected int w;
    protected int h;
    protected Image tiles;
    protected TrueTypeFont ttf;
    protected Action action;
            
    protected GuiElement parent = null;

    public GuiElement(Image tiles, TrueTypeFont ttf, int x, int y, int w, int h) {
        this.x = x*SC;
        this.y = y*SC;
        this.w = w*SC;
        this.h = h*SC;
        this.tiles = tiles;
        this.ttf = ttf;
    }

    public GuiElement(int x, int y, int w, int h) {
        this.x = x*SC;
        this.y = y*SC;
        this.w = w*SC;
        this.h = h*SC;
    }
    
    public GuiElement() {      
    }

    public GuiElement getParent() {
        return parent;
    }

    public void setParent(GuiElement parent) {
        this.parent = parent;
    }

    public void setAction(Action action) {
        this.action = action;
    }
       
    public boolean isInside(int x, int y) {
        return (x >= this.x && x <= this.x + w && y >= this.y && y<= this.y+h);
    }
    
    public void click(int x, int y, int button) {
        System.out.println("Click: x="+x+" y="+y+" button="+button);
        System.out.println(this.getClass().getCanonicalName()+": x="+this.x+" y="+this.y+" w="+this.w+" h="+this.h);
        System.out.println(parent.getClass().getCanonicalName()+": x="+parent.x+" y="+parent.y+" w="+parent.w+" h="+parent.h);
    }

    //in tile coords (x=0-20, y=0-16) relative to window origin
    public void drawTile(Image tiles, int xt, int yt, int tile) {
        int ty = tile/20;
        int tx = tile%20;
        tiles.drawEmbedded(xt*SC+x,yt*SC+y, xt*SC+SC+x,yt*SC+SC+y, tx*SC,ty*SC, tx*SC+SC,ty*SC+SC);       
    }       
    
    public Image getTile(Image tiles, int tile) {
        int ty = tile/20;
        int tx = tile%20;
        return tiles.getSubImage(tx*SC,ty*SC, SC, SC);
    }
    
    public void drawString(int xt, int yt, String string) {
        ttf.drawString(xt*SC+x+1,yt*SC+y+1, string, Color.black);
        ttf.drawString(xt*SC+x-1,yt*SC+y-1, string, Color.black);
        ttf.drawString(xt*SC+x+1,yt*SC+y-1, string, Color.black);
        ttf.drawString(xt*SC+x-1,yt*SC+y+1, string, Color.black);
        ttf.drawString(xt*SC+x,yt*SC+y, string, Color.red);
    }     
    
    public void drawTile(Image tiles, int xt, int yt, int tile, int brightness) {
        int ty = tile/20;
        int tx = tile%20;
        int c = brightness;
        Color col = new Color(c,c,c);
        tiles.drawEmbedded(xt*SC+x,yt*SC+y, xt*SC+SC+x,yt*SC+SC+y, tx*SC,ty*SC, tx*SC+SC,ty*SC+SC,col);       
    }        
}
