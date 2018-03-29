CREATE OR REPLACE FUNCTION generate_sample_by_selectivity (selectivity INTEGER) 
	RETURNS TABLE (O_CUSTKEY INT) as $$
	DECLARE rd_key INT := 0;
	DECLARE negReg INT := 0;
	DECLARE tamCustomer INT := 0;
	DECLARE tamOrders INT := 0;
	DECLARE range INT := 0;
BEGIN

	--CALCULATES THE NUMBER OF REGISTERS NEEDED
	tamCustomer := (SELECT COUNT(*) FROM CUSTOMER);
	negReg := ((100-selectivity)*tamCustomer)/100;
	tamOrders := (SELECT COUNT(*) FROM ORDERS);
	IF negReg = 0 THEN
		range := tamOrders;
	ELSE
		range := tamOrders/negReg::INT;
	END IF;


	--REGISTERS COINTAINED ON CUSTOMERS
	DROP TABLE IF EXISTS tmp_orders;
	CREATE TEMPORARY TABLE tmp_orders (O_CUSTKEY INT);

	FOR i IN 1..range LOOP
		INSERT INTO tmp_orders
			SELECT DISTINCT 
				C_CUSTKEY 
			FROM CUSTOMER LIMIT negReg;
	END LOOP;

	INSERT INTO tmp_orders
	SELECT DISTINCT 
		C_CUSTKEY 
	FROM CUSTOMER LIMIT tamOrders-(negReg*range);

	RETURN QUERY SELECT 
		tmp_orders.O_CUSTKEY
	FROM tmp_orders;
END;

$$ LANGUAGE plpgsql;
