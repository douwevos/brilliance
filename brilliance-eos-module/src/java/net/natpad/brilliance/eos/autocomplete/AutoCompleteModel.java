package net.natpad.brilliance.eos.autocomplete;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import com.tickeos.core.service.impl.module.dto.ArticleDto;
import com.tickeos.core.service.impl.module.dto.BooleanTypeDto;
import com.tickeos.core.service.impl.module.dto.DynamicProductPropertyDto;
import com.tickeos.core.service.impl.module.dto.DynamicProductPropertyGroupDto;
import com.tickeos.core.service.impl.module.dto.EnumerationTypeDto;
import com.tickeos.core.service.impl.module.dto.ImageTypeDto;
import com.tickeos.core.service.impl.module.dto.IntegerTypeDto;
import com.tickeos.core.service.impl.module.dto.ModuleDto;
import com.tickeos.core.service.impl.module.dto.NamedEntityDto;
import com.tickeos.core.service.impl.module.dto.ProductDto;
import com.tickeos.core.service.impl.module.dto.ProductPropertyTemplateDto;
import com.tickeos.core.service.impl.module.dto.PropertyTypeDto;
import com.tickeos.core.service.impl.module.dto.PropertyValueDto;
import com.tickeos.core.service.impl.module.dto.SpatialValidityDescriptorDto;
import com.tickeos.core.service.impl.module.dto.TextTypeDto;
import com.tickeos.core.service.impl.module.dto.TransportProductDto;
import com.tickeos.core.shared.BuiltInPropertyType;
import com.tickeos.core.shared.PropertyValueSource;
import com.tickeos.core.shared.StaticProperty;

import net.natpad.brilliance.Logger;
import net.natpad.json.mapper.JsonMapper;

public class AutoCompleteModel implements Route, Logger {

	private final JsonMapper mapper;
	private final ModuleDto moduleDto;
	
	private final TypeRegistry typeRegistry;
	
	private final Map<String, ProductPropertyReference> propertiesMap = new HashMap<>();
	
	private final Map<String, PropertyValueReference> propertyValuesMap = new HashMap<>();
	
	private ArticleDto article;
	private ProductDto product;
	
	public AutoCompleteModel(JsonMapper mapper, ModuleDto moduleDto) {
		this.mapper = mapper;
		this.moduleDto = moduleDto;
		typeRegistry = new TypeRegistry();
		moduleDto.getEnumerationTypes().stream().map(s -> new TypeDescriptionPropertyEnumeration(typeRegistry, s)).forEach(typeRegistry::register);
	}
	
	
	public void buildForArticle(ArticleDto articleDto) {
		this.article = articleDto;
		TransportProductDto productDto = moduleDto.getTransportProducts().stream().filter(s -> s.getArticles().stream().anyMatch(a -> a==articleDto)).findAny().orElse(null);
		buildForTransportProduct(productDto);
		
		articleDto.getPropertyValues().stream().forEach(p -> propertyValuesMap.put(p.getSymbolicName(), new PropertyValueReference(p)));
	}

	public void buildForTransportProduct(TransportProductDto productDto) {
		this.product = productDto;
		buildPropertiesMap(productDto);
		productDto.getPropertyValues().stream().forEach(p -> propertyValuesMap.put(p.getSymbolicName(), new PropertyValueReference(p)));
	}
	
	public TypeRegistry getTypeRegistry() {
		return typeRegistry;
	}
	
	@Override
	public String name() {
		return null;
	}
	
	@Override
	public TypeDescription type() {
		return null;
	}
	
	
	@Override
	public Map<String, Route> children() {
		Map<String, Route> root = new HashMap<String, Route>();
		propertyValuesMap.forEach((k,v) -> {
			PropertyTypeDto propertyType = findPropertyType(k);
			TypeDescription typeDescription = asTypeDescription(propertyType);
			if (typeDescription != null) {
				TypeField typeField = new TypeField(typeDescription, k);
				root.put(k, typeField);
			} else {
				log("unknown type:"+propertyType+" : "+(propertyType==null ? "" : propertyType.getSymbolicName()));
			}
		});
		
		root.put("requestedStart", new TypeField(typeRegistry.getType("StopPoint"), "requestedStart"));
//		root.put("propertyValues", new TypeField(typeRegistry.getType("StopPoint"), "requestedStart"));
		
		List<TypeField> arguments = new ArrayList<TypeField>();
		arguments.add(new TypeField(typeRegistry.getType("LocalDateTime"), "date"));
		arguments.add(new TypeField(typeRegistry.getType("String"), "pattern"));
		TypeMethod typeMethod = new TypeMethod(typeRegistry.getType("String"), "formatDate", arguments );
		root.put("formatDate", typeMethod);
		return root;
	}
	
	public List<PropertyNameAndSource> getDefinedPropertyNamesAndSourceList(boolean forArticle) {
		List<PropertyNameAndSource> names = new ArrayList<PropertyNameAndSource>();
		propertiesMap.forEach((k,v) -> {
			PropertyValueSource source = v.getSource();
			if (forArticle) {
				if (source!=PropertyValueSource.PRODUCT) {
					names.add(new PropertyNameAndSource(k, source));
				}
			} else {
				if (source==PropertyValueSource.PRODUCT || source==PropertyValueSource.PREDEFINED) {
					names.add(new PropertyNameAndSource(k, source));
				}
			}
		});

		Collections.sort(names);
		return names;
	}
	
	public static class PropertyNameAndSource implements Comparable<PropertyNameAndSource>{
		
		public final String name;
		public final PropertyValueSource source;
		
		PropertyNameAndSource(String name, PropertyValueSource source) {
			this.name = name;
			this.source = source;
		}
		
		@Override
		public int compareTo(PropertyNameAndSource o) {
			return name.compareTo(o.name);
		}
	}
	
	public Set<String> getDefinedPropertyValueNames() {
		return propertyValuesMap.keySet();
	}
	
	private TypeDescription asTypeDescription(PropertyTypeDto propertyType) {
		if (propertyType == null) {
			return null;
		}
		return typeRegistry.getType(propertyType.getSymbolicName());
	}

	
	public PropertyValueDto findPropertyValueByName(String name) {
		PropertyValueReference reference = propertyValuesMap.get(name);
		return reference==null ? null : reference.dto;
	}

	public PropertyTypeDto findPropertyType(String name) {
		ProductPropertyReference reference = propertiesMap.get(name);
		return reference==null ? null : reference.getResultType(this);
	}
	
	public Object findPropertyTypeDeclaration(String name) {
		ProductPropertyReference reference = propertiesMap.get(name);
		return reference.getDeclaration();
	}

	public List<PropertyValueDto> listPropertyValues() {
		return propertyValuesMap.values().stream().map(p -> p.dto).collect(Collectors.toList());
	}
	
	private void buildPropertiesMap(TransportProductDto productDto) {
		List<String> propertyGroups = productDto.getPropertyGroups();
		for(String propertyGroupName : propertyGroups) {
			DynamicProductPropertyGroupDto group = findProductPropertyGroup(propertyGroupName);
			group.getProperties().stream().forEach(pp -> propertiesMap.put(pp.getSymbolicName(), new DynamicProductPropertyReference(group, pp)));
		}
		
//		String periodOfValidityDescriptorName = productDto.getPeriodOfValidityDescriptor();
//		if (periodOfValidityDescriptorName!=null) {
//			PeriodOfValidityDescriptorDto validityDescriptorDto = moduleDto.getPeriodOfValidityDescriptors().stream().filter(s -> Objects.equals(s.getSymbolicName(), periodOfValidityDescriptorName)).findAny().orElse(null);
//			if (validityDescriptorDto!=null) {
//				validityDescriptorDto.getDuration().
//			}
//		}
		
		String spatialValidityDescriptor = productDto.getSpatialValidityDescriptor();
		log("spatialValidityDescriptor="+spatialValidityDescriptor);
		if (spatialValidityDescriptor != null) {
			SpatialValidityDescriptorDto spatialValidityDescriptorDto = moduleDto.getSpatialValidityDescriptors().stream().filter(s -> Objects.equals(s.getSymbolicName(), spatialValidityDescriptor)).findAny().orElse(null);
			storeTemplateProperty(StaticProperty.AREA, spatialValidityDescriptorDto.getArea());
			storeTemplateProperty(StaticProperty.REQUESTED_START, spatialValidityDescriptorDto.getStart());
			storeTemplateProperty(StaticProperty.REQUESTED_DESTINATION, spatialValidityDescriptorDto.getDestination());
		}
	}
	
	private void storeTemplateProperty(StaticProperty staticProperty, ProductPropertyTemplateDto templateDto) {
		log("staticProperty="+staticProperty+", templateDto="+templateDto);
		if (templateDto == null) {
			return;
		}
		propertiesMap.put(staticProperty.getSymbolicName(), new StaticProductPropertyReference(staticProperty, templateDto));
	}


	public DynamicProductPropertyGroupDto findProductPropertyGroup(String name) {
		List<DynamicProductPropertyGroupDto> dynamicProductPropertyGroups = moduleDto.getDynamicProductPropertyGroups();
		return dynamicProductPropertyGroups.stream().filter(s -> Objects.equals(s.getSymbolicName(), name)).findAny().orElse(null);
	}


	public List<StaticFunction> enlistFunctions() {
		List<StaticFunction> result = new ArrayList<StaticFunction>();

		// Strings
		result.add(new StaticFunction("formatSequence", "String", "formatSequence(iterable, ',')"));
		result.add(new StaticFunction("getFirstString", "String", "getFirstString(value, ',', -1)"));
		result.add(new StaticFunction("getLastString", "String", "getLastString(value, ',', -1)"));
		result.add(new StaticFunction("getLastString", "String", "wrap(value, 'pre', 'post')"));

		// ArrayFunctions
		result.add(new StaticFunction("get", "String", "get(list, index)"));
		
		
		// Dates
		result.add(new StaticFunction("formatDate", "String", "formatDate(date, 'dd-mm-yyyy')"));
		return result;
	}
	

	public static class StaticFunction {
		
		public final String name;
		public final String returnType;
		public final String template;
		
		public StaticFunction(String name, String returnType, String template) {
			this.name = name;
			this.returnType = returnType;
			this.template = template;
		}
	}
	
	public static interface ProductPropertyReference {

		String getName();
		
		PropertyValueSource getSource();

		Object getDeclaration();

		PropertyTypeDto getResultType(AutoCompleteModel model);
	}

	public static class DynamicProductPropertyReference implements ProductPropertyReference {
		private final DynamicProductPropertyGroupDto group;
		private final DynamicProductPropertyDto productProperty;

		public DynamicProductPropertyReference(DynamicProductPropertyGroupDto group, DynamicProductPropertyDto productProperty) {
			this.group = group;
			this.productProperty = productProperty;
		}
		
		public PropertyTypeDto getResultType(AutoCompleteModel model) {
			String type = productProperty.getType();
			return model.resolveType(type);
		}
		
		@Override
		public String getName() {
			return productProperty.getSymbolicName();
		}
		
		@Override
		public Object getDeclaration() {
			return productProperty;
		}
		
		@Override
		public PropertyValueSource getSource() {
			return productProperty.getSource();
		}
	}

	public static class StaticProductPropertyReference implements ProductPropertyReference {

		private final StaticProperty staticProperty;
		private final ProductPropertyTemplateDto templateDto;
		
		public StaticProductPropertyReference(StaticProperty staticProperty, ProductPropertyTemplateDto templateDto) {
			this.staticProperty = staticProperty;
			this.templateDto = templateDto;
		}

		public PropertyValueSource getSource() {
			return templateDto.getSource();
		}
		
		@Override
		public PropertyTypeDto getResultType(AutoCompleteModel model) {
			String type = staticProperty.getSymbolicName();
			return model.resolveType(templateDto.getType());
		}
		
		@Override
		public String getName() {
			return staticProperty.getSymbolicName();
		}
	
		@Override
		public Object getDeclaration() {
			return templateDto;
		}
		
	}

	
	public static class PropertyValueReference {
		
		private final PropertyValueDto dto;
		
		public PropertyValueReference(PropertyValueDto dto) {
			this.dto = dto;
		}
		
		public String getName() {
			return dto.getSymbolicName();
		}
		
		
	}


	public PropertyTypeDto resolveType(String type) {
		BuiltInPropertyType builtInPropertyType = BuiltInPropertyType.bySymbolicName(type);
		if (builtInPropertyType != null) {
			PropertyTypeDto result = new PropertyTypeDto(type, type, true);
			return result;
		}

		Predicate<? super NamedEntityDto> symbolicNameFilter = s -> Objects.equals(s.getSymbolicName(), type);

		BooleanTypeDto boolType = moduleDto.getBooleanTypes().stream().filter(symbolicNameFilter).findAny().orElse(null);
		if (boolType!=null) {
			return boolType;
		}
		
		EnumerationTypeDto enumerationTypeDto = moduleDto.getEnumerationTypes().stream().filter(symbolicNameFilter).findAny().orElse(null);
		if (enumerationTypeDto!=null) {
			return enumerationTypeDto;
		}
		
		ImageTypeDto imageTypeDto = moduleDto.getImageTypes().stream().filter(symbolicNameFilter).findAny().orElse(null);
		if (imageTypeDto!=null) {
			return imageTypeDto;
		}
		
		IntegerTypeDto integerTypeDto = moduleDto.getIntegerTypes().stream().filter(symbolicNameFilter).findAny().orElse(null);
		if (integerTypeDto!=null) {
			return integerTypeDto;
		}
		
		TextTypeDto textTypeDto = moduleDto.getTextTypes().stream().filter(symbolicNameFilter).findAny().orElse(null);
		if (textTypeDto!=null) {
			return textTypeDto;
		}
		
		return null;
	}


	
}
