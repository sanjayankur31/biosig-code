
all: test

%:%.c 
	$(CC) -DHL1 $< -lbiosig -lm -o $@.HL1
	$(CC) -DHL2 $< -lbiosig -lm -o $@.HL2
	$(CC) -DLL1 $< -lbiosig -lm -o $@.LL1
	$(CC) -DLL2 $< -lbiosig -lm -o $@.LL2
	
	
test: generate_gdf_file
	-./generate_gdf_file.HL1 hl1.gdf
	-./generate_gdf_file.HL2 hl2.gdf
	-./generate_gdf_file.LL1 ll1.gdf
	-./generate_gdf_file.LL2 ll2.gdf
	
clean:
	rm *.gdf generate_gdf_file.[HL]L[12]
		
