package producerCustomer;

public class Data {
    private String message;
    private int id;

    public Data(int number){
        this.message = "THIS IS DATA";
        this.id = number;
    }

    public String talk(){
        return message + " " + id;
    }
}
