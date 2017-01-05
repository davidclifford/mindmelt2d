package mindmelt.gui;

public class Zone extends GuiElement {

    public Zone(int x, int y, int w, int h) {
        super(x, y, w, h);
    }   
    
    @Override
    public void click(int x, int y, int button) {
        int subZone = (x-this.x)/SC+((((y-this.y)/SC) % (this.w/SC))*(this.h/SC));
        System.out.println("SubZone: x="+(x-this.x)/SC+" y="+(y-this.y)/SC+" zone="+subZone);
        action.doAction(subZone);
    }
}
