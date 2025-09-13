public class WeatherDay {
    private int id; //1 through 365 or 366 if data is over a leap year
    private String fullDate;
    private int day;
    private int month;
    private double rainTotal;

    public WeatherDay(int id, int day, int month, double rainTotal){
        this.id = id;
        this.day = day;
        this.month = month;
        this.rainTotal = rainTotal;
    }//end WeatherDay constructor

    //mutators
    public void setId(int id){
        this.id = id;
    }

    public void setDay(int day){
        this.day = day;
    }

    public void setMonth(int month){
        this.month = month;
    }

    public void setRainTotal(double rainTotal){
        this.rainTotal = rainTotal;
    }

    //accessors
    public int getId(){
        return id;
    }

    public int getDay(){
        return day;
    }

    public int getMonth(){
        return month;
    }

    public double getRainTotal(){
        return rainTotal;
    }

    //methods
    public String toString(){
        return "The " + day + " day of the " + month + " month had " + rainTotal + " inches of rain at this station.";
    }
}
