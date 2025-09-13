import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Label;
import javafx.stage.Stage;

import java.io.File;
import java.io.FileNotFoundException;
import java.net.URL;
import java.util.ResourceBundle;
import java.util.Scanner;

public class ForestFormController implements Initializable {
    private Stage stage;
    private final WeatherDay[] YearOfData = new WeatherDay[366]; //size is, for one year of data, maximum
    private int arrayCounter = 0;
    private final String[] Months = {"January", "February", "March", "April", "May", "June", "July",
                                      "August", "September", "October", "November", "December"};
    @FXML
    private Label LocationName;

    @FXML
    private Label StationName;

    @FXML
    private Label StationLocation;

    @FXML
    private Label DataYear;

    @FXML
    private BarChart<String, Integer> TotalDaysWithoutRain; //For every month, total days w/o rain

    @FXML
    private BarChart<String, Double> TotalPrecipitation; //For every month, total precipitation

    @FXML
    private BarChart<String, Integer> TotalConsecutiveDaysWithoutRain; //For every month, total consecutive days w/o rain

    @FXML
    private BarChart<String, Double> AveragePrecipitationPerRainyDay; //For every month, the average amount of rain for a rainy day

    @FXML
    private BarChart<String, Double> PrecipitationTotalsPerWeek; //For each week of the year, total precipitation for the week

    //FXML for predictive charts
    @FXML
    private BarChart<String, Double> januaryPredictive;

    @FXML
    private BarChart<String, Double> februaryPredictive;

    @FXML
    private BarChart<String, Double> marchPredictive;

    @FXML
    private BarChart<String, Double> aprilPredictive;

    @FXML
    private BarChart<String, Double> mayPredictive;

    @FXML
    private BarChart<String, Double> junePredictive;

    @FXML
    private BarChart<String, Double> julyPredictive;

    @FXML
    private BarChart<String, Double> augustPredictive;

    @FXML
    private BarChart<String, Double> septemberPredictive;

    @FXML
    private BarChart<String, Double> octoberPredictive;

    @FXML
    private BarChart<String, Double> novemberPredictive;

    @FXML
    private BarChart<String, Double> decemberPredictive;


    @Override
    public void initialize(URL url, ResourceBundle rb){


        //read CSV file, place daily values into WeatherDay Object, fill array with all the WeatherDay Objects
        try {
            File weatherData = new File("2899680.csv");
            Scanner scan = new Scanner(weatherData);
            scan.nextLine();//first line is for humans, not machines
            int id = 1;
            int tempDay;
            int tempMonth;
            double tempRainTotal = 0.0;
            while(scan.hasNext()){
                String dayData = scan.next();
                //System.out.println(dayData);
                Scanner dayScan = new Scanner(dayData);
                dayScan.useDelimiter("/");
                tempMonth = dayScan.nextInt();
                //System.out.println("month: " + tempMonth);
                tempDay = dayScan.nextInt();
                //System.out.println("day: " + tempDay);
                dayScan.useDelimiter(",");
                dayScan.next();//year, not used
                tempRainTotal = dayScan.nextDouble();//rain
                //System.out.println("rain: " + tempRainTotal);
                dayScan.close();

                WeatherDay tempWeatherDay = new WeatherDay(id, tempDay, tempMonth, tempRainTotal);
                //System.out.println("WeatherDay: " + tempWeatherDay.toString());
                YearOfData[arrayCounter] = tempWeatherDay;
                id++;
                arrayCounter++;
            }
            scan.close();
        } catch (FileNotFoundException e){
            System.out.println("File Not Found!");
            e.printStackTrace();
        }
        //quickly test that the array has everything
        //int i = 0;
        //while(i < arrayCounter){
        //    System.out.println(YearOfData[i].toString());
        //    i++;
        //}

        //****************************************************************
        //Create relevant data for all charts
        //****************************************************************

        XYChart.Series<String,Double> totalPrecipitation = new XYChart.Series<>();
        XYChart.Series<String,Double> averagePrecipitationPerRainyDay = new XYChart.Series<>();
        XYChart.Series<String, Integer> totalDaysWithoutRain = new XYChart.Series<>();
        XYChart.Series<String, Integer> totalConsecutiveDaysWithoutRain = new XYChart.Series<>();
        int i = 0;
        double totalRain = 0.0;
        int daysWithoutRain = 0;
        int daysWithRain = 0;
        boolean wasPreviousDayRainFree = false;
        int tempDaysWithoutRain = 0;
        int consecutiveDaysWithoutRain = 0;
        int currentMonth = 1;
        while(i < arrayCounter){
            while(i < arrayCounter && YearOfData[i].getMonth() == currentMonth){
                if(YearOfData[i].getRainTotal() > 0.0){
                    daysWithRain++;
                    totalRain += YearOfData[i].getRainTotal();
                    if(tempDaysWithoutRain > 0){
                        tempDaysWithoutRain++;
                        consecutiveDaysWithoutRain += tempDaysWithoutRain;
                        tempDaysWithoutRain = 0;
                    }
                }else{
                    daysWithoutRain++;
                    if(wasPreviousDayRainFree){
                        tempDaysWithoutRain++;
                    }
                }
                wasPreviousDayRainFree = YearOfData[i].getRainTotal() == 0;
                i++;
            }//end month of data
            consecutiveDaysWithoutRain += tempDaysWithoutRain;
            totalConsecutiveDaysWithoutRain.getData().add(new XYChart.Data<>(Months[(currentMonth - 1)],consecutiveDaysWithoutRain));
            totalPrecipitation.getData().add(new XYChart.Data<>(Months[(currentMonth - 1)], totalRain));
            if(daysWithRain > 0){
                averagePrecipitationPerRainyDay.getData().add(new XYChart.Data<>(Months[(currentMonth - 1)], (totalRain / daysWithoutRain)));
            }else{
                averagePrecipitationPerRainyDay.getData().add(new XYChart.Data<>(Months[(currentMonth - 1)], 0.0));
            }
            totalDaysWithoutRain.getData().add(new XYChart.Data<>(Months[(currentMonth - 1)], daysWithoutRain));
            currentMonth++;
            totalRain = 0.0;
            daysWithRain = 0;
            daysWithoutRain = 0;
            tempDaysWithoutRain = 0;
            consecutiveDaysWithoutRain = 0;
        }//end year of data

        //Add data to charts
        TotalDaysWithoutRain.setTitle("Total number of days without rain");
        TotalDaysWithoutRain.getData().add(totalDaysWithoutRain);
        TotalPrecipitation.setTitle(("Total precipitation"));
        TotalPrecipitation.getData().add(totalPrecipitation);
        TotalConsecutiveDaysWithoutRain.setTitle("Total number of consecutive days without rain");
        TotalConsecutiveDaysWithoutRain.getData().add(totalConsecutiveDaysWithoutRain);
        AveragePrecipitationPerRainyDay.setTitle("Average precipitation per rainy day");
        AveragePrecipitationPerRainyDay.getData().add(averagePrecipitationPerRainyDay);

        //Precipitation totals for entire year but by week
        //BarChart<Integer, Double> PrecipitationTotalsPerWeek;
        //XYChart.Series<String, Integer> decemberTotalConsecutiveDaysWithoutRain = new XYChart.Series<>();

        XYChart.Series<String, Double> weeklyPrecipitation = new XYChart.Series<>();
        i = 0;
        double weeklyRainTotal = 0.0;
        int week = 1;
        int dayOfTheWeek = 1;
        while(i < arrayCounter && week <= 52){
            while(dayOfTheWeek <= 7 && i < arrayCounter){
                weeklyRainTotal += YearOfData[i].getRainTotal();
                dayOfTheWeek++;
                i++;
            }
            //System.out.println("Rain totals for week " + week + ": " + weeklyRainTotal);
            weeklyPrecipitation.getData().add(new XYChart.Data<>("" + week, weeklyRainTotal));
            //decemberTotalConsecutiveDaysWithoutRain.getData().add(new XYChart.Data<>("December", consecutiveDaysWithoutRain));
            dayOfTheWeek = 1;
            weeklyRainTotal = 0.0;
            week++;
        }
        PrecipitationTotalsPerWeek.getData().add(weeklyPrecipitation);


        //Now, for the predictive element!
        //TODO check that this is accurately calculating linear regression using the data
        //TODO because it's close but not the same as the excel calculation using the same data.
        //TODO that doesn't mean it isn't correct, but that does mean it might not be correct
        //Linear Regression for each month
            //use the month of data to create a linear regression line's formula
        XYChart.Series<String, Double> januaryPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> februaryPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> marchPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> aprilPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> mayPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> junePrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> julyPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> augustPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> septemberPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> octoberPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> novemberPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double> decemberPrediction = new XYChart.Series<>();
        XYChart.Series<String, Double>[] yearPredictionData = new XYChart.Series[12];
        yearPredictionData[0] = januaryPrediction;
        yearPredictionData[1] = februaryPrediction;
        yearPredictionData[2] = marchPrediction;
        yearPredictionData[3] = aprilPrediction;
        yearPredictionData[4] = mayPrediction;
        yearPredictionData[5] = junePrediction;
        yearPredictionData[6] = julyPrediction;
        yearPredictionData[7] = augustPrediction;
        yearPredictionData[8] = septemberPrediction;
        yearPredictionData[9] = octoberPrediction;
        yearPredictionData[10] = novemberPrediction;
        yearPredictionData[11] = decemberPrediction;


        double meanX = 0.0;
        double meanY = 0.0;
        double varianceX;
        double covariance;
        currentMonth = 1;
        int currentDay = 1;
        i = 0;
        int j;
        int daysInMonthCount;
        while(i < arrayCounter) {
            daysInMonthCount = 0;
            varianceX = 0.0;
            //double varianceY = 0.0;
            covariance = 0.0;
            //mean of X and Y for the month
            while (i < arrayCounter && YearOfData[i].getMonth() == currentMonth) {
                j = i;
                while(j < arrayCounter && YearOfData[j].getMonth() == currentMonth){
                    meanX += YearOfData[j].getDay();
                    meanY += YearOfData[j].getRainTotal();
                    j++;
                    daysInMonthCount++;
                }
                meanX = meanX / daysInMonthCount;
                meanY = meanY / daysInMonthCount;
                System.out.println("Mean for X and Y: " + meanX + ", " + meanY);
                //Variance of X and covariance for the month
                while(i < arrayCounter && YearOfData[i].getMonth() == currentMonth){
                    varianceX += Math.pow( (YearOfData[i].getDay() - meanX), 2);
                    //varianceY += Math.pow( (YearOfData[i].getRainTotal() - meanY), 2);
                    covariance += ( (YearOfData[i].getDay() - meanX) * (YearOfData[i].getRainTotal() - meanY));
                    i++;
                }
                //now, calculate b1 and b0 for the linear equation of the linear regression
                covariance = (covariance) / (daysInMonthCount);
                System.out.println("Covariance: " + covariance + ", varianceX: " + varianceX);
                double b1 = covariance / varianceX;
                double b0 = meanY - (b1 * meanX);
                System.out.println("b1: " + b1 + ", b0: " + b0);
                double predictedRainfall;
                //y = b0 + (b1 * x)
                //where y is the rainfall prediction for day of the month x
                //yearPredictionData[0].getData().add
                //totalDaysWithoutRain.getData().add(new XYChart.Data<>(Months[(currentMonth - 1)], daysWithoutRain));
                while(currentDay <= daysInMonthCount){
                    predictedRainfall = (b0 + (b1 * currentDay));
                    yearPredictionData[(currentMonth - 1)].getData().add(new XYChart.Data<>("" + currentDay, predictedRainfall) );
                    System.out.println("Prediction for " + currentDay + ": " + predictedRainfall);
                    currentDay++;
                }
                currentDay = 1;
            }//end of month
            currentMonth++;
        }//end of year

        //okay, with that all done we should have a nice array full of data things for each month, each full of predictions for each day.
        //so now we can update each chart using this data and, if I've actually done this correctly, it'll all look nice a pretty. yay
        januaryPredictive.setTitle("Predicted rainfall in inches");
        januaryPredictive.getData().add(yearPredictionData[0]);
        februaryPredictive.setTitle("Predicted rainfall in inches");
        februaryPredictive.getData().add(yearPredictionData[1]);
        marchPredictive.setTitle("Predicted rainfall in inches");
        marchPredictive.getData().add(yearPredictionData[2]);
        aprilPredictive.setTitle("Predicted rainfall in inches");
        aprilPredictive.getData().add(yearPredictionData[3]);
        mayPredictive.setTitle("Predicted rainfall in inches");
        mayPredictive.getData().add(yearPredictionData[4]);
        junePredictive.setTitle("Predicted rainfall in inches");
        junePredictive.getData().add(yearPredictionData[5]);
        julyPredictive.setTitle("Predicted rainfall in inches");
        julyPredictive.getData().add(yearPredictionData[6]);
        augustPredictive.setTitle("Predicted rainfall in inches");
        augustPredictive.getData().add(yearPredictionData[7]);
        septemberPredictive.setTitle("Predicted rainfall in inches");
        septemberPredictive.getData().add(yearPredictionData[8]);
        octoberPredictive.setTitle("Predicted rainfall in inches");
        octoberPredictive.getData().add(yearPredictionData[9]);
        novemberPredictive.setTitle("Predicted rainfall in inches");
        novemberPredictive.getData().add(yearPredictionData[10]);
        decemberPredictive.setTitle("Predicted rainfall in inches");
        decemberPredictive.getData().add(yearPredictionData[11]);



        //y = b0 + ( b1 * x )
        //We now have the equation of the line, which means we can predict for any x (day) in the set (month)
        //double tempY = b0 + (b1 * 31);
        //System.out.println("Our estimated rainfall for Jan 31st: " + tempY + " inches");

        //set form labels
        DataYear.setText("Data from year: 2021");
        LocationName.setText("Forest Grove 1.3 NNW, OR US");
        StationName.setText("US1ORWS0140");
        StationLocation.setText("Latitude 45.538807, Longitude -123.115421");
    }//end initialize

}
