package DomainDiagram;

public interface PaymentClient {

	boolean authorisePayment();

	boolean requestRefund();

}