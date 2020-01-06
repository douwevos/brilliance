package net.natpad.json.mapper;

import java.io.File;
import java.io.Reader;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectWriter;
import com.tickeos.core.service.impl.module.dto.ModuleDto;
import com.tickeos.core.service.impl.module.dto.TransportProductDto;

public class MapperTest {

	public static void log(String txt) {
		System.out.println(txt);
	}
	
	public static void main(String[] args) {
//		doParseTest();
		doParseModule();
//		doParseTransportProduct();

	}
	
	
	private static void doParseModule() {
		try {
		
			ObjectMapper mapper = new ObjectMapper();
			ModuleDto m = mapper.readValue(new File("/home/dvos/workspaces/openshop/eos-ts-openshop/etc/config/pm/vrr_module.json"), ModuleDto.class);
			
//			ModuleDto m = new ModuleDto();
//			m.setSymbolicName("vrr09");
//			m.setDisplayName("Vrr version 9");
			
			
			
			List<TransportProductDto> transportProducts = new ArrayList<TransportProductDto>();
			TransportProductDto product = new TransportProductDto();
			product.setSymbolicName("productje");
			transportProducts.add(product);
			m.setTransportProducts(transportProducts);
			
			ObjectWriter objectWriter = mapper.writerWithDefaultPrettyPrinter();
			String moduleAsJson = objectWriter.writeValueAsString(m);
			log("t="+moduleAsJson);

			JsonMapper jsonMapper = new JsonMapper(new StringReader(moduleAsJson));
			ModuleDto test = jsonMapper.map(ModuleDto.class);
			log("test="+test);

			moduleAsJson = objectWriter.writeValueAsString(test);
			log("reparsed="+moduleAsJson);

			
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}

	private static void doParseTransportProduct() {
		try {
		
			
			TransportProductDto product = new TransportProductDto();
			product.setSymbolicName("productje");
			
			ObjectMapper mapper = new ObjectMapper();
			ObjectWriter objectWriter = mapper.writerWithDefaultPrettyPrinter();
			String moduleAsJson = objectWriter.writeValueAsString(product);
			log("t="+moduleAsJson);

			JsonMapper jsonMapper = new JsonMapper(new StringReader(moduleAsJson));
			TransportProductDto test = jsonMapper.map(TransportProductDto.class);
			log("test="+test);

			moduleAsJson = objectWriter.writeValueAsString(test);
			log("reparsed="+moduleAsJson);

			
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}

	private static void doParseTest() {
		Reader reader = new StringReader("{\"name\" : \"Douwe\", \"hallo\" : [ { \"hop\" : \"waarde\"} ] }")  ;
		try {
			JsonMapper jsonMapper = new JsonMapper(reader);
			Test test = jsonMapper.map(Test.class);
			log("test="+test);
		
			ObjectMapper mapper = new ObjectMapper();
			ObjectWriter objectWriter = mapper.writerWithDefaultPrettyPrinter();
			String t = objectWriter.writeValueAsString(test);
			log("t="+t);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	


	public static class Test {
		String name;
		public List<Sub> hallo;
		public List<Awah> wahs;
		
		public String getName() {
			return name;
		}
		
		public void setName(String name) {
			this.name = name;
		}
		
		@Override
		public String toString() {
			return "Test[name="+name+"]";
		}
	}

	public static class Sub {
		public String hop;
		public String par;
	}

	public static class Awah {
		public String foo;
		public String bar;
	}

}
